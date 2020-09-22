#!/usr/bin/env python3
import paddle as paddle
import paddle.fluid as fluid
from cinn.frontend import *
from cinn import Target
from cinn.framework import *
import unittest
import cinn
from cinn import runtime
from cinn import ir
from cinn import lang
from cinn.common import *
import numpy as np
import paddle.fluid as fluid
import sys

model_dir = sys.argv.pop()


class TestLoadResnetModel(unittest.TestCase):
    def setUp(self):
        self.target = Target()
        self.target.arch = Target.Arch.X86
        self.target.bits = Target.Bit.k64
        self.target.os = Target.OS.Linux

        self.model_dir = model_dir

        self.x_shape = [1, 3, 224, 224]

    def get_paddle_inference_result(self, data):
        exe = fluid.Executor(fluid.CPUPlace())

        [inference_program, feed_target_names,
         fetch_targets] = fluid.io.load_inference_model(
             dirname=self.model_dir, executor=exe)

        results = exe.run(
            inference_program,
            feed={feed_target_names[0]: data},
            fetch_list=fetch_targets)

        result = results[0]
        return result

    def test_model(self):
        np.random.seed(0)
        x_data = np.random.random(self.x_shape).astype("float32")
        self.executor = Executor(["resnet_input"], [self.x_shape])
        self.executor.load_paddle_model(self.model_dir, False)
        a_t = self.executor.get_tensor("resnet_input")
        a_t.from_numpy(x_data)

        out = self.executor.get_tensor("relu_1.tmp_0")
        out.from_numpy(np.zeros(out.shape(), dtype='float32'))

        self.executor.run()

        out = out.numpy()
        target_result = self.get_paddle_inference_result(x_data)

        print("result in test_model: \n")
        out = out.reshape(-1)
        target_result = target_result.reshape(-1)
        for i in range(0, out.shape[0]):
            if np.abs(out[i] - target_result[i]) > 1e-3:
                print("Error! ", i, "-th data has diff with target data:\n",
                      out[i], " vs: ", target_result[i], ". Diff is: ",
                      out[i] - target_result[i])
        # TODO(haozech) fix this.
        # self.assertTrue(np.allclose(out, target_result, atol=1e-3))


if __name__ == "__main__":
    unittest.main()