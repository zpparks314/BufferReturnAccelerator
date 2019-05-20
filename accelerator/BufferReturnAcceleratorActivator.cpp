#include "cppmicroservices/BundleActivator.h"
#include "cppmicroservices/BundleContext.h"
#include "cppmicroservices/ServiceProperties.h"
#include "XACC.hpp"
#include "BufferReturnAccelerator.hpp"

using namespace cppmicroservices;

class US_ABI_LOCAL BufferReturnAcceleratorActivator : public BundleActivator {

public:
        BufferReturnAcceleratorActivator() {}
        void Start(BundleContext context) {
          auto acc = std::make_shared<xacc::quantum::BufferReturnAccelerator>();

          context.RegisterService<xacc::Accelerator>(acc);
          context.RegisterService<xacc::OptionsProvider>(acc);
        }

        void Stop(BundleContext context) {}
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(BufferReturnAcceleratorActivator)
