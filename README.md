# NLY

## target

* header-only.
* cross-platform.
* simple to use.



## how to use

* You can copy `nly/nly` to your project, because `nly` is header-only project.

  * If you use `nly` in this way, then you need to manually include the third-party libraries required by `nly`.

* You can put nly to your cmake project, for example:

  ```cmake
  cmake_minimum_required(VERSION 3.0.0)
  project(your_target VERSION 0.1.0)
  
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
  
  add_subdirectory(nly)
  add_executable(your_target xxx.cpp)
  target_link_libraries(your_target PRIVATE nly)
  ```



##  third-party libraries

* The third-party libraries that `nly` depends on are all disabled by default, and can be enabled according to the user's needs. The following is the list of third-party libraries and the variable names used to enable them.

| name  | variable          |
| ----- | ----------------- |
| fmt   | ADD_FMT_VIA_NLY   |
| boost | ADD_BOOST_VIA_NLY |



### update submodule for boost

```shell
# nly/upstream/boost
$ git submodule update --init --recursive
```



## test

* Set `ENABLE_NLY_UNIT_TEST` to `ON` to enable unit test.
