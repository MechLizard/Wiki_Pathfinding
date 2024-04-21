{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/addon.cpp", "../src/Article.cpp",  "../src/main.cpp", "../src/WikiImporter.cpp"],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../src"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ]
    }
  ]
}
