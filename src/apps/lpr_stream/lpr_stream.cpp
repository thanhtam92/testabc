
#include <mqtt/mqtt_baseapp.h>
#include <pipeline/pipeline.h>

namespace onevu {

class LPRStreamApplication : public MqttBaseApplication {

public:
    LPRStreamApplication() : MqttBaseApplication("lpr_stream") {}

private:
    int main(const std::vector<std::string> &args){

        LOG_INFO("Starting %s...", this->_name.c_str());

        onevu::Pipeline pipeline = onevu::Pipeline::create();
        pipeline.start();


        // wait for CTRL-C or kill
        waitForTerminationRequest();

        pipeline.stop();
        
        return Application::EXIT_OK;
    }
};

} // namespace onevu

POCO_SERVER_MAIN(onevu::LPRStreamApplication)

