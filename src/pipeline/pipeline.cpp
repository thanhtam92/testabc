#include "pipeline.h"
#include "trigger/lpdtrigger_generator.h"

namespace onevu {

Pipeline Pipeline::create() {
    Pipeline pipeline;

    // create node
    auto filterparam = std::make_unique<FrameFilterParameter>();
    
    auto input = std::make_unique<node::InputStream>();
    auto aggrtrig = std::make_unique<node::TriggerAggregator>();
    auto preselect = std::make_unique<node::PreSelection>();
    auto lpd = std::make_unique<node::LicensePlateDetector>();
    auto filter = std::make_unique<node::FrameFilter>(std::move(filterparam));
    auto vd = std::make_unique<node::VehicleDetector>();
    auto roimap = std::make_unique<node::RoiMapping>();

    // create trigger
    trigger::LPDTriggerGeneratorParameter lpd_trig_param{};
    auto lpdtrigGen = std::make_unique<trigger::LPDTriggerGenerator>(lpd_trig_param);

    // create and link queue to nodes
    // aggregatortrigger get input from stream
    input->queueIn0 = std::make_shared<FrameNotificationQueue>();
    input->queueOut0 = std::make_shared<FrameNotificationQueue>();
    input->queueOut1 = lpdtrigGen->queueIn;
    aggrtrig->queueIn0 = input->queueOut0;

    // preselect input from aggregatortrigger
    aggrtrig->queueOut0 = std::make_shared<FrameNotificationQueue>();
    preselect->queueIn0 = aggrtrig->queueOut0;

    // lpd input from preselection
    preselect->queueOut0 = std::make_shared<FrameNotificationQueue>();
    preselect->queueOut1 = std::make_shared<FrameNotificationQueue>();
    lpd->queueIn0 = preselect->queueOut0;
    // recording->queueIn0 = preselect->queueOut1;

    // filter input from lpd
    lpd->queueOut0 = std::make_shared<FrameNotificationQueue>();
    filter->queueIn0 = lpd->queueOut0;

    // vd input from filter
    filter->queueOut0 = std::make_shared<FrameNotificationQueue>();
    vd->queueIn0 = filter->queueOut0;

    // roi input from vd
    vd->queueOut0 = std::make_shared<FrameNotificationQueue>();
    roimap->queueIn0 = vd->queueOut0;
    roimap->queueOut0 = std::make_shared<FrameNotificationQueue>();

    

    // store node
    pipeline._listNode.push_back(std::move(input));
    pipeline._listNode.push_back(std::move(aggrtrig));
    pipeline._listNode.push_back(std::move(preselect));
    pipeline._listNode.push_back(std::move(filter));
    pipeline._listNode.push_back(std::move(lpd));
    pipeline._listNode.push_back(std::move(vd));
    pipeline._listNode.push_back(std::move(roimap));
    
    pipeline._listTriggerGenerator.push_back(std::move(lpdtrigGen));

    return std::move(pipeline);
}

void Pipeline::start() {

    if(this->_isRunning) {
        LOG_INFO("Pipeline is running!!!");
        return;
    }

    for(auto &triggen : _listTriggerGenerator){
        if(triggen)
            triggen->start();
    }

    _pipelineThread = std::thread([&](){
        this->run();
    });
}

void Pipeline::stop() {
    for(auto &triggen : _listTriggerGenerator){
        if(triggen)
            triggen->stop();
    }
    this->_isRunning = false;
    this->_pipelineThread.join();
}

void Pipeline::run() {
    _isRunning = true;
    while(_isRunning){
        auto start_time = milliseconds_since_epoch();
        
        for(auto &node : _listNode){
            if(node){
                if(node->preProcess()){
                    node->process();
                    node->postProcess();
                }
            }
        }

        //
        auto process_time = milliseconds_since_epoch() - start_time;
        // LOG_INFO("processed time: %ld", process_time);
        if(process_time < 5){
            std::this_thread::sleep_for(std::chrono::milliseconds(5 - process_time));
        }
    }
}

}