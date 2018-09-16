# Amun 
Stand-alone C++ decoder for RNN-based NMT models. Can decode with default models from Marian and Nematus.

[![CUDABuild Status](http://vali.inf.ed.ac.uk/jenkins/buildStatus/icon?job=amunmt_compilation_cuda)](http://vali.inf.ed.ac.uk/jenkins/job/amunmt_compilation_cuda/)
[![CPU Build Status](http://vali.inf.ed.ac.uk/jenkins/buildStatus/icon?job=amunmt_compilation_cpu)](http://vali.inf.ed.ac.uk/jenkins/job/amunmt_compilation_cpu/)
[![Twitter](https://img.shields.io/twitter/follow/marian_nmt.svg?style=social&label=Follow)](https://twitter.com/intent/follow?screen_name=marian_nmt)

This tool was a part of [Marian](https://github.com/marian-nmt/marian), which now has its own decoder. Therefore, amun has been moved to a separate repository. For RNN models this decoder can still be faster on the GPU than the official Marian decoder.

If you use this, please cite:

Marcin Junczys-Dowmunt, Roman Grundkiewicz, Tomasz Dwojak, Hieu Hoang, Kenneth Heafield, Tom Neckermann, Frank Seide, Ulrich Germann, Alham Fikri Aji, Nikolay Bogoychev, André F. T. Martins, Alexandra Birch (2018). Marian: Fast Neural Machine Translation in C++ (http://www.aclweb.org/anthology/P18-4020)

    @InProceedings{mariannmt,
        title     = {Marian: Fast Neural Machine Translation in {C++}},
        author    = {Junczys-Dowmunt, Marcin and Grundkiewicz, Roman and
                     Dwojak, Tomasz and Hoang, Hieu and Heafield, Kenneth and
                     Neckermann, Tom and Seide, Frank and Germann, Ulrich and
                     Fikri Aji, Alham and Bogoychev, Nikolay and
                     Martins, Andr\'{e} F. T. and Birch, Alexandra},
        booktitle = {Proceedings of ACL 2018, System Demonstrations},
        pages     = {116--121},
        publisher = {Association for Computational Linguistics},
        year      = {2018},
        month     = {July},
        address   = {Melbourne, Australia},
        url       = {http://www.aclweb.org/anthology/P18-4020}
    }

## Website

More information on https://marian-nmt.github.io

- [Quick start](https://marian-nmt.github.io/quickstart)
- [Installation and usage documentation](https://marian-nmt.github.io/docs)
- [Usage examples](https://marian-nmt.github.io/examples)

## Acknowledgements

The development of Marian received funding from the European Union's
_Horizon 2020 Research and Innovation Programme_ under grant agreements
688139 ([SUMMA](http://www.summa-project.eu); 2016-2019),
645487 ([Modern MT](http://www.modernmt.eu); 2015-2017),
644333 ([TraMOOC](http://tramooc.eu/); 2015-2017),
644402 ([HiML](http://www.himl.eu/); 2015-2017),
the Amazon Academic Research Awards program,
the World Intellectual Property Organization,
and is based upon work supported in part by the Office of the Director of
National Intelligence (ODNI), Intelligence Advanced Research Projects Activity
(IARPA), via contract #FA8650-17-C-9117.

This software contains source code provided by NVIDIA Corporation.
