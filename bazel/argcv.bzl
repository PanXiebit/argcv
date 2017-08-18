# How to prepare rules: https://bazel.build/versions/master/docs/skylark/cookbook.html

# Parse the bazel version string from `native.bazel_version`.
def _parse_bazel_version(bazel_version):
  # Remove commit from version.
  version = bazel_version.split(" ", 1)[0]

  # Split into (release, date) parts and only return the release
  # as a tuple of integers.
  parts = version.split('-', 1)

  # Turn "release" into a tuple of strings
  version_tuple = ()
  for number in parts[0].split('.'):
    version_tuple += (str(number),)
  return version_tuple

# Check that a specific bazel version is being used.
def check_version(bazel_version):
  if "bazel_version" not in dir(native):
    fail("\nCurrent Bazel version is lower than 0.2.1, expected at least %s\n" % bazel_version)
  elif not native.bazel_version:
    print("\nCurrent Bazel is not a release version, cannot check for compatibility.")
    print("Make sure that you are running at least Bazel %s.\n" % bazel_version)
  else:
    current_bazel_version = _parse_bazel_version(native.bazel_version)
    minimum_bazel_version = _parse_bazel_version(bazel_version)
    if minimum_bazel_version > current_bazel_version:
      fail("\nCurrent Bazel version is {}, expected at least {}\n".format(
          native.bazel_version, bazel_version))
  pass

##########################################################################################
########### temp_workaround_http_archive start ###########################################
##########################################################################################
# for _temp_workaround_http_archive_impl
def _repos_are_siblings():
  return Label("@foo//bar").workspace_root.startswith("../")

# for temp_workaround_http_archive
# Temporary workaround to support including TensorFlow as a submodule until this
# use-case is supported in the next Bazel release.
def _temp_workaround_http_archive_impl(repo_ctx):
   repo_ctx.template("BUILD", repo_ctx.attr.build_file,
                     {
                         "%prefix%" : ".." if _repos_are_siblings() else "external",
                         "%ws%": repo_ctx.attr.repository
                     }, False)
   repo_ctx.download_and_extract(repo_ctx.attr.urls, "", repo_ctx.attr.sha256,
                                 "", repo_ctx.attr.strip_prefix)

temp_workaround_http_archive = repository_rule(
    attrs = {
        "build_file": attr.label(),
        "repository": attr.string(),
        "urls": attr.string_list(default = []),
        "sha256": attr.string(default = ""),
        "strip_prefix": attr.string(default = ""),
    },
    implementation = _temp_workaround_http_archive_impl,
)

##########################################################################################
########### temp_workaround_http_archive end #############################################
##########################################################################################

# If TensorFlow is linked as a submodule.
# path_prefix and tf_repo_name are no longer used.
def argcv_deps():
  # print("argcv configuring deps starting...")
  # cuda_configure(name = "local_config_cuda")
  # sycl_configure(name = "local_config_sycl")

  # Eigen is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
  # For more information go to http://eigen.tuxfamily.org/.
  native.new_http_archive(
      name = "eigen_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/bitbucket.org/eigen/eigen/get/174e09eed96c.tar.gz",
          "https://bitbucket.org/eigen/eigen/get/174e09eed96c.tar.gz",
      ],
      sha256 = "38d4bda36435cd40e5235887cca8f108f8125d5a28eac471555efdf9d259020f",
      strip_prefix = "eigen-eigen-174e09eed96c",
      build_file = str(Label("//third_party:eigen.BUILD")),
  )

  # ref: https://github.com/hfp/libxsmm
  # LIBXSMM is a library for small dense and small sparse matrix-matrix multiplications as well
  # as for deep learning primitives such as small convolutions targeting Intel Architecture.
  # Small matrix multiplication kernels are generated for the following instruction set extensions:
  # Intel SSE, Intel AVX, Intel AVX2, IMCI (KNCni) for Intel Xeon Phi coprocessors ("KNC"), and
  # Intel AVX‑512 as found in the Intel Xeon Phi processor family (Knights Landing "KNL", Knights
  # Mill "KNM") and Intel Xeon processors (Skylake-E "SKX"). Historically small matrix multiplications
  # were only optimized for the Intel Many Integrated Core Architecture "MIC") using intrinsic functions,
  # meanwhile optimized assembly code is targeting all afore mentioned instruction set extensions
  # (static code generation), and Just‑In‑Time (JIT) code generation is targeting Intel AVX and beyond.
  # Optimized code for small convolutions is JIT-generated for Intel AVX2 and Intel AVX‑512.
  native.new_http_archive(
      name = "libxsmm_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/hfp/libxsmm/archive/1.7.1.tar.gz",
          "https://github.com/hfp/libxsmm/archive/1.7.1.tar.gz",
      ],
      sha256 = "9d3f63ce3eed62f04e4036de6f2be2ce0ff07781ca571af6e0bf85b077edf17a",
      strip_prefix = "libxsmm-1.7.1",
      build_file = str(Label("//third_party:libxsmm.BUILD")),
  )

  native.bind(
      name = "xsmm_avx",
      actual = "@libxsmm_archive//third_party:xsmm_avx",
  )

  # https://github.com/google/re2
  # RE2 is a fast, safe, thread-friendly alternative to backtracking regular expression engines
  # like those used in PCRE, Perl, and Python. It is a C++ library.
  native.http_archive(
      name = "com_googlesource_code_re2",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/google/re2/archive/b94b7cd42e9f02673cd748c1ac1d16db4052514c.tar.gz",
          "https://github.com/google/re2/archive/b94b7cd42e9f02673cd748c1ac1d16db4052514c.tar.gz",
      ],
      sha256 = "bd63550101e056427c9e7ff12a408c1c8b74e9803f393ca916b2926fc2c4906f",
      strip_prefix = "re2-b94b7cd42e9f02673cd748c1ac1d16db4052514c",
  )
  
  # this is my bind of re2
  native.bind(
      name = "re2",
      actual = "@com_googlesource_code_re2//third_party:re2",
  )

  native.new_http_archive(
      name = "nasm",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/www.nasm.us/pub/nasm/releasebuilds/2.12.02/nasm-2.12.02.tar.bz2",
          "http://pkgs.fedoraproject.org/repo/pkgs/nasm/nasm-2.12.02.tar.bz2/d15843c3fb7db39af80571ee27ec6fad/nasm-2.12.02.tar.bz2",
      ],
      sha256 = "00b0891c678c065446ca59bcee64719d0096d54d6886e6e472aeee2e170ae324",
      strip_prefix = "nasm-2.12.02",
      build_file = str(Label("//third_party:nasm.BUILD")),
  )

  # gtest
  native.new_http_archive(
      name = "gmock_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/google/googletest/archive/release-1.8.0.zip",
          "https://github.com/google/googletest/archive/release-1.8.0.zip",
      ],
      sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
      strip_prefix = "googletest-release-1.8.0",
      build_file = str(Label("//third_party:gmock.BUILD")),
  )

  # gtest
  native.bind(
      name = "gtest",
      actual = "@gmock_archive//:gtest",
  )

  # gtest main
  native.bind(
      name = "gtest_main",
      actual = "@gmock_archive//:gtest_main",
  )
  
  # Description:
  # Snappy is a compression/decompression library 
  native.new_http_archive(
      name = "snappy_archive",
      urls = [
          "https://github.com/google/snappy/releases/download/1.1.4/snappy-1.1.4.tar.gz",
      ],
      sha256 = "134bfe122fd25599bb807bb8130e7ba6d9bdb851e0b16efcb83ac4f5d0b70057",
      strip_prefix = "snappy-1.1.4",
      build_file = str(Label("//third_party:snappy.BUILD")),
  )
  
  # gtest main
  native.bind(
      name = "snappy",
      actual = "@snappy_archive//:snappy",
  )
  
  # Description:
  # LevelDB is a fast key-value storage library written at Google that
  # provides an ordered mapping from string keys to string values.
  native.new_http_archive(
      name = "leveldb_archive",
      urls = [
          "https://github.com/google/leveldb/archive/v1.20.tar.gz",
      ],
      sha256 = "f5abe8b5b209c2f36560b75f32ce61412f39a2922f7045ae764a2c23335b6664",
      strip_prefix = "leveldb-1.20",
      build_file = str(Label("//third_party:leveldb.BUILD")),
  )
  
  # gtest main
  native.bind(
      name = "leveldb",
      actual = "@leveldb_archive//:leveldb",
  )

  # Description:
  #   Six provides simple utilities for wrapping over differences between Python 2
  #   and Python 3.
  native.new_http_archive(
      name = "six_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/pypi.python.org/packages/source/s/six/six-1.10.0.tar.gz",
          "http://pypi.python.org/packages/source/s/six/six-1.10.0.tar.gz",
      ],
      sha256 = "105f8d68616f8248e24bf0e9372ef04d3cc10104f1980f54d57b2ce73a5ad56a",
      strip_prefix = "six-1.10.0",
      build_file = str(Label("//third_party:six.BUILD")),
  )

  # Bind of six
  native.bind(
      name = "six",
      actual = "@six_archive//:six",
  )
  
  # for python
  native.bind(
      name = "python_headers",
      actual = str(Label("//util/python:python_headers")),
  )

  # curl
  temp_workaround_http_archive(
      name = "curl_archive",
      sha256 = "ff3e80c1ca6a068428726cd7dd19037a47cc538ce58ef61c59587191039b2ca6",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/curl.haxx.se/download/curl-7.49.1.tar.gz",
          "https://curl.haxx.se/download/curl-7.49.1.tar.gz",
      ],
      strip_prefix = "curl-7.49.1",
      build_file = str(Label("//third_party:curl.BUILD")),
      repository = "",
  )
  
  native.bind(
      name = "curl",
      actual = "@curl_archive//:curl",
  )

  # It is requreid by grpc
  # We need to import the protobuf library under the names com_google_protobuf
  # and com_google_protobuf_cc to enable proto_library support in bazel.
  # Unfortunately there is no way to alias http_archives at the moment.
  # update note: I have already removed related deps
  native.http_archive(
      name = "com_google_protobuf",
      urls = [
          "http://mirror.bazel.build/github.com/google/protobuf/archive/0b059a3d8a8f8aa40dde7bea55edca4ec5dfea66.tar.gz",
          "https://github.com/google/protobuf/archive/0b059a3d8a8f8aa40dde7bea55edca4ec5dfea66.tar.gz",
      ],
      sha256 = "6d43b9d223ce09e5d4ce8b0060cb8a7513577a35a64c7e3dad10f0703bf3ad93",
      strip_prefix = "protobuf-0b059a3d8a8f8aa40dde7bea55edca4ec5dfea66",
  )

  # grpc expects //external:protobuf_clib and //external:protobuf_compiler
  # to point to the protobuf's compiler library.
  # note: update: use protobuf_compiler only
  native.bind(
      name = "protobuf_compiler",
      actual = "@com_google_protobuf//:protoc_lib",
  )

  native.bind(
      name = "protobuf_clib",
      actual = "@com_google_protobuf//:protoc_lib",
  )

  native.bind(
      name = "protoc",
      actual = "@com_google_protobuf//:protoc",
  )
  
  # TODO protobuf_java, protobuf_java_util etc.

  native.new_http_archive(
      name = "grpc",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/grpc/grpc/archive/d7ff4ff40071d2b486a052183e3e9f9382afb745.tar.gz",
          "https://github.com/grpc/grpc/archive/d7ff4ff40071d2b486a052183e3e9f9382afb745.tar.gz",
      ],
      sha256 = "a15f352436ab92c521b1ac11e729e155ace38d0856380cf25048c5d1d9ba8e31",
      strip_prefix = "grpc-d7ff4ff40071d2b486a052183e3e9f9382afb745",
      build_file = str(Label("//third_party:grpc.BUILD")),
  )

  # protobuf expects //external:grpc_cpp_plugin to point to grpc's
  # C++ plugin code generator.
  native.bind(
      name = "grpc_cpp_plugin",
      actual = "@grpc//:grpc_cpp_plugin",
  )

  native.bind(
      name = "grpc_lib",
      actual = "@grpc//:grpc++_unsecure",
  )
  
  # GRPC C++ runtime library
  native.bind(
    name = "grpc++",
    actual = "@grpc//:grpc++"
  )

  native.bind(
    name = "grpc++_codegen_proto",
    actual = "@grpc//:grpc++_codegen_proto"
  )

  # branch: master-with-bazel
  native.http_archive(
      name = "boringssl",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/google/boringssl/archive/bbcaa15b0647816b9a1a9b9e0d209cd6712f0105.tar.gz",
          "https://github.com/google/boringssl/archive/bbcaa15b0647816b9a1a9b9e0d209cd6712f0105.tar.gz",  # 2016-07-11
      ],
      sha256 = "025264d6e9a7ad371f2f66d17a28b6627de0c9592dc2eb54afd062f68f1f9aa3",
      strip_prefix = "boringssl-bbcaa15b0647816b9a1a9b9e0d209cd6712f0105",
  )
  
  native.bind(
    name = "libssl",
    actual = "@boringssl//:ssl"
  )
  
  native.bind(
    name = "libssl_objc",
    actual = "@boringssl//:libssl_objc"
  )

  # Protocol Buffers with small code size https://jpa.kapsi.fi/nanopb/
  native.new_http_archive(
      name = "nanopb_git",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/nanopb/nanopb/archive/1251fa1065afc0d62f635e0f63fec8276e14e13c.tar.gz",
          "https://github.com/nanopb/nanopb/archive/1251fa1065afc0d62f635e0f63fec8276e14e13c.tar.gz",
      ],
      sha256 = "ab1455c8edff855f4f55b68480991559e51c11e7dab060bbab7cffb12dd3af33",
      strip_prefix = "nanopb-1251fa1065afc0d62f635e0f63fec8276e14e13c",
      build_file = str(Label("//third_party:nanopb.BUILD")),
  )

  native.bind(
      name = "nanopb",
      actual = "@nanopb_git//:nanopb",
  )

  # zlib is designed to be a free, general-purpose, legally unencumbered -- that is, not covered by any patents 
  # -- lossless data-compression library for use on virtually any computer hardware and operating system. 
  # The zlib data format is itself portable across platforms. Unlike the LZW compression method used in Unix compress(1) 
  # and in the GIF image format, the compression method currently used in zlib essentially never expands the data
  #. (LZW can double or triple the file size in extreme cases.) zlib's memory footprint is also independent of the input 
  # data and can be reduced, if necessary, at some cost in compression. A more precise, technical discussion 
  # of both points is available on another page.
  native.new_http_archive(
      name = "zlib_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/zlib.net/zlib-1.2.8.tar.gz",
          "http://zlib.net/fossils/zlib-1.2.8.tar.gz",
      ],
      sha256 = "36658cb768a54c1d4dec43c3116c27ed893e88b02ecfcb44f2166f9c0b7f2a0d",
      strip_prefix = "zlib-1.2.8",
      build_file = str(Label("//third_party:zlib.BUILD")),
  )

  native.bind(
      name = "zlib",
      actual = "@zlib_archive//:zlib",
  )

  native.new_http_archive(
      name = "gflags_archive",
      build_file = str(Label("//third_party:gflags.BUILD")), # Use Customized Build File
      sha256 = "99f9e8b63ea53e4b23f0fd0f68a6f1e397d5512be36716c17cc75966a90f0d57",
      strip_prefix = "gflags-2.2.0",
      url = "https://github.com/gflags/gflags/archive/v2.2.0.zip",
  )

  native.bind(
    name = "gflags",
    actual = "@gflags_archive//:gflags",
  )

  native.new_local_repository(
    name = "glog_config",
    path = "third_party/glog",
    build_file = "third_party/glog/BUILD",
  )

  native.new_http_archive(
    name = "glog_archive",
    build_file = str(Label("//third_party:glog.BUILD")),
    sha256 = "b2f35994772423a6f51e885e57a345350bac70a5aee8e035428643c0a0a4ca30",
    strip_prefix = "glog-da816ea70645e463aa04f9564544939fa327d5a7",
    url = "https://github.com/google/glog/archive/da816ea70645e463aa04f9564544939fa327d5a7.zip",
  )
  
  native.bind(
    name = "glog",
    actual = "@glog_archive//:glog",
  )

  # # based on https://github.com/nelhage/rules_boost
  native.new_http_archive(
    name = "boost",
    url = "https://nelhage.s3.amazonaws.com/rules_boost/boost_1_63_0.tar.bz2",
    # url = "https://doc.argcv.com/repo/bazel/boost_1_63_0.tar.bz2",
    build_file = "third_party/boost.BUILD",  #str(Label("//third_party:boost.BUILD")),
    strip_prefix = "boost_1_63_0/",
    sha256 = "beae2529f759f6b3bf3f4969a19c2e9d6f0c503edcb2de4a61d1428519fcb3b0",
  )
  
  native.new_http_archive(
      name = "rapidjson_archive",
      urls = [
          "https://github.com/miloyip/rapidjson/archive/v1.1.0.tar.gz",
      ],
      sha256 = "bf7ced29704a1e696fbccf2a2b4ea068e7774fa37f6d7dd4039d0787f8bed98e",
      strip_prefix = "rapidjson-1.1.0",
      build_file = str(Label("//third_party:rapidjson.BUILD")),
  )
  
  native.bind(
    name = "rapidjson",
    actual = "@rapidjson_archive//:rapidjson",
  )
# # print("argcv configuring deps finished...")
