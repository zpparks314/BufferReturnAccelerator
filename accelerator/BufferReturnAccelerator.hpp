#ifndef BUFF_RETURN_ACCELERATOR_HPP
#define BUFF_RETURN_ACCELERATOR_HPP

#include "XACC.hpp"
#include "Accelerator.hpp"

using namespace xacc;

namespace xacc { namespace quantum {

class BufferReturnAccelerator : public Accelerator {

public:

  BufferReturnAccelerator() : Accelerator() {}

  void initialize() override {}

  std::vector<ExtraInfo> params;

  OptionPairs getOptions() override {
    OptionPairs desc {{"return-buffer", ""}, {"return-params", ""}};
    return desc;
  }

  std::shared_ptr<AcceleratorBuffer> createBuffer(const std::string& varId) override;

  std::shared_ptr<AcceleratorBuffer> createBuffer(const std::string& varId, const int size) override;

  bool isValidBufferSize(const int nbits) override;

  void execute(std::shared_ptr<AcceleratorBuffer> buffer,
        const std::shared_ptr<Function> kernel) {
        auto buffers = execute(buffer, std::vector<std::shared_ptr<xacc::Function>>{kernel});
    }

std::vector<std::shared_ptr<AcceleratorBuffer>> execute(
                std::shared_ptr<AcceleratorBuffer> buffer,
                const std::vector<std::shared_ptr<Function>> functions) override;

  AcceleratorType getType() override { return AcceleratorType::qpu_gate; }


  const std::string name() const override {
    return "buffer-return";
  }

  const std::string description() const override {
    return "The Buffer-Return Accelerator returns a non-executed copy of the provided buffer, "
              "including all children buffers.";
  }

  std::vector<std::shared_ptr<xacc::IRTransformation>> getIRTransformations() override {
    std::vector<std::shared_ptr<xacc::IRTransformation>> v;
    return v;
  }

  ~BufferReturnAccelerator() {}

};
}}

#endif