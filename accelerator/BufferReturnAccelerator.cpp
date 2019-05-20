#include "BufferReturnAccelerator.hpp"
#include "XACC.hpp"

namespace xacc { namespace quantum {


std::shared_ptr<AcceleratorBuffer> BufferReturnAccelerator::createBuffer(const std::string& varId) { return createBuffer(varId, 100); }


std::shared_ptr<AcceleratorBuffer> BufferReturnAccelerator::createBuffer(const std::string& varId, const int size) {
  if (!isValidBufferSize(size)) {
    xacc::error("BufferReturnAccelerator - Invalid buffer size.");
  }
  auto buffer = std::make_shared<AcceleratorBuffer>(varId, size);
  storeBuffer(varId, buffer);
  return buffer;
}

bool BufferReturnAccelerator::isValidBufferSize(const int nBits) { return nBits <= 1000; }

// const std::vector<std::shared_ptr<AcceleratorBuffer>> BufferReturnAccelerator::execute(
//                                       std::shared_ptr<AcceleratorBuffer> buffer,
//                                       const std::vector<std::shared_ptr<Function>> functions,
//                                       std::vector<double> _params) {
//                     for (auto &p : _params){
//                         params.push_back(ExtraInfo(p));
//                     }
//                     auto buffers = execute(buffer, functions);
//                     return buffers;
//             }


std::vector<std::shared_ptr<AcceleratorBuffer>> BufferReturnAccelerator::execute(
                                      std::shared_ptr<AcceleratorBuffer> buffer,
                                      const std::vector<std::shared_ptr<Function>> functions) {

                    std::vector<std::shared_ptr<AcceleratorBuffer>> returnBuffers;
                    if (!xacc::optionExists("return-buffer")) {
                        xacc::error("Cannot use BufferReturnAccelerator without a return-buffer option.");
                    }
                    auto fileStr = xacc::getOption("return-buffer");
                    std::ifstream t(fileStr);
                    std::string returnBufferFile((std::istreambuf_iterator<char>(t)),
                                    std::istreambuf_iterator<char>());

                    std::istringstream is(returnBufferFile);
                    auto loadedBuffer = std::make_shared<AcceleratorBuffer>();
                    loadedBuffer->load(is);

                    // for (auto &fun : functions){
                    //     xacc::info(fun->toString("q"));
                    //     xacc::info("numparams: " + std::to_string(fun->nParameters()));
                    //     if (fun->nParameters() > 0){
                    //     auto param = fun->getParameter(0).as<std::complex<double>>();
                    //     xacc::info("param" + std::to_string(param.real()));
                    //     }
                    // }
                    std::vector<std::vector<double>> params;
                    auto paramStr = xacc::getOption("return-params");
                    xacc::info(paramStr);
                    std::vector<std::string> split = xacc::split(paramStr, ',');
                    for (auto &s : split){
                        params.push_back({std::stod(s)});
                    }

                    // auto testParams = loadedBuffer->getAllUnique("parameters");
                    // for (auto &m : testParams){
                    //     std::cout << m.index() << std::endl;
                    // }
                    for (auto &p : params) {
                        std::vector<std::shared_ptr<AcceleratorBuffer>> add;
                        auto children = loadedBuffer->getChildren("parameters", p);
                        std::cout << children.size() << std::endl;
                        for (auto &c : children){
                            if ("I" != mpark::get<std::string>(c->getInformation("kernel"))) {
                            returnBuffers.push_back(c);
                        }
                    }
                }
                    return returnBuffers;

}

// void BufferReturnAccelerator::execute(std::shared_ptr<AcceleratorBuffer> buffer, const std::shared_ptr<Function> kernel) {

// }

}}
