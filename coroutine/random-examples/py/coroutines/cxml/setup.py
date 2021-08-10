from distutils.core import setup,Extension
setup(name="cxmlparse",
      ext_modules=[Extension("cxmlparse",[
                                        "cxmlparse.c",
                                        ],
                             include_dirs=['/usr/local/include'],
                             library_dirs=['/usr/local/lib'],
                             libraries=['expat'],
                             )]
     )



                    
