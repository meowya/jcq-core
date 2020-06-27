
#include "javacq/core/core.h"

#include "javacq/components/config/default_config.h"
#include "javacq/components/config/ini_config.h"

#include "javacq/components/resources/default_resources.h"

#include "javacq/components/dispatcher/jni_dispatcher.h"


using namespace jcq;

CQ_INIT {

    init();

    // config
    use(components::ini_config);
    use(components::default_config);

    // resources
    use(components::default_resources);

    // java core
    use(components::jni_dispatcher);

}
