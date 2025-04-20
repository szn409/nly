# NLY

## 目标

* 主体代码：header-only

* 支持跨平台
* 良好的测试用例覆盖



## 依赖的第三方库环境

* 方式一：用户自行保证
* 方式二：
  * 使用 [nly_upstream](https://github.com/szn409/nly_upstream.git)
  * 本项目的测试用例采用此方式搭建
* BS_thread_pool.hpp
  * 拷贝自 https://github.com/bshoshany/thread-pool/releases/tag/v5.0.0





## 测试用例

* 位于 tests 文件夹中

* 运行测试用例（以 vs2019 环境为例）

  * CMakeSettings.json

    ```json
    {
      "configurations": [
        {
          "name": "x64-Debug",
          "generator": "Visual Studio 16 2019 Win64",
          "configurationType": "Debug",
          "inheritEnvironments": [ "msvc_x64_x64" ],
          "buildRoot": "${projectDir}\\out\\build\\${name}",
          "installRoot": "${projectDir}\\out\\install\\${name}",
          "cmakeCommandArgs": "-DCMAKE_BUILD_TYPE=Debug -DENABLE_NLY_UNIT_TEST=ON -DNLY_UPSTREAM_INSTALL=D:/nly_upstream/nly_install_d",
          "ctestCommandArgs": ""
        },
        {
          "name": "x64-Release",
          "generator": "Visual Studio 16 2019 Win64",
          "configurationType": "Release",
          "buildRoot": "${projectDir}\\out\\build\\${name}",
          "installRoot": "${projectDir}\\out\\install\\${name}",
          "cmakeCommandArgs": "-DCMAKE_BUILD_TYPE=Release -DENABLE_NLY_UNIT_TEST=ON -DNLY_UPSTREAM_INSTALL=D:/nly_upstream/nly_install_r",
          "buildCommandArgs": "",
          "ctestCommandArgs": "",
          "inheritEnvironments": [ "msvc_x64_x64" ]
        }
      ]
    }
    ```

  * 注意：

    * ENABLE_NLY_UNIT_TEST：设置为 ON
    * NLY_UPSTREAM_INSTALL：设置为 nly_upstream 的 install 路径



## CMake

* 主体 target 名称：nly

* 使用方式：参考 tests/CMakeLists.txt



## 跨平台测试

| 平台       | 编译器     | 结果 |
| ---------- | ---------- | ---- |
| windows 10 | VS2019     | ✅    |
| ubuntu     | GCC 11.4.0 | ⭕️    |

* ✅：已支持
*  ⭕️：可能支持，但尚未测试

