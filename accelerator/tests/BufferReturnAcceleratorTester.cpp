#include "BufferReturnAccelerator.hpp"
#include "XACC.hpp"
#include "xacc_service.hpp"
#include <gtest/gtest.h>


using namespace xacc::quantum;

const std::string rucc = R"rucc(def f(buffer, theta):
    X(0)
    X(1)
    Rx(1.5707,0)
    H(1)
    H(2)
    H(3)
    CNOT(0,1)
    CNOT(1,2)
    CNOT(2,3)
    Rz(theta,3)
    CNOT(2,3)
    CNOT(1,2)
    CNOT(0,1)
    Rx(-1.5707,0)
    H(1)
    H(2)
    H(3)
    )rucc";

TEST(BufferReturnAcceleratorTester, simpleCheck) {

    xacc::setOption("return-buffer", "/home/rdm_generation/return_buffer_accelerator/project/accelerator/tests/nah_rdm_purification_local-ibm_vqe_20190513-214725.ab");
    auto acc = xacc::getAccelerator("buffer-return");
    auto fileStr = xacc::getOption("return-buffer");
    std::ifstream t(fileStr);
    std::string returnBufferFile((std::istreambuf_iterator<char>(t)),
                                  std::istreambuf_iterator<char>());
    std::istringstream is(returnBufferFile);
    auto loadedBuffer = std::make_shared<xacc::AcceleratorBuffer>();
    loadedBuffer->load(is);
    auto params = loadedBuffer->getAllUnique("parameters");
    std::vector<std::shared_ptr<AcceleratorBuffer>> testChildren;

    for (auto &p : params) {
            std::vector<std::shared_ptr<AcceleratorBuffer>> add;
            auto children = loadedBuffer->getChildren("parameters", p);
            for (auto &c : children){
                testChildren.push_back(c);
            }
        }
    int n_test_children = testChildren.size();

    std::vector<std::shared_ptr<Function>> dummyFsToExecute;
    auto compiler = xacc::getService<xacc::Compiler>("xacc-py");

    auto ir = compiler->compile(rucc, acc);
    auto ruccsd = ir->getKernel("f");

    std::vector<double> parameters{-1.0899653409569644};
    ruccsd = (*ruccsd.get())(parameters);

    auto ir2 = compiler->compile(rucc, acc);
    auto ruccsd2 = ir2->getKernel("f");

    ruccsd2 = (*ruccsd2.get())(parameters);
    dummyFsToExecute.push_back(ruccsd);
    dummyFsToExecute.push_back(ruccsd2);

    std::string paramStr;
    std::stringstream ss;
    for (auto &p : parameters){
        ss << std::setprecision(16) << p;
    }
    paramStr = ss.str();
    xacc::setOption("return-params", paramStr);
    auto buffer = acc->createBuffer("q", 4);
    auto buffers = acc->execute(buffer, dummyFsToExecute);

    int n_children = buffers.size();
    std::cout << std::to_string(n_children) << std::endl;
    // EXPECT_EQ(n_test_children, n_children);

    // for (int i; i = 0; i < testChildren.size()){
    //     EXPECT_EQ(testChildren[i]->name(), buffers[i]->name());
    // }

}


int main(int argc, char **argv) {
  xacc::Initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  xacc::Finalize();
}
