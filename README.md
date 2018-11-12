# Amun

[![CPU Build Status](https://img.shields.io/jenkins/s/http/vali.inf.ed.ac.uk/jenkins/view/amunmt/job/amun-cpu.svg?label=CPU%20Build)](http://vali.inf.ed.ac.uk/jenkins/job/amun-cpu/)
[![CUDA Build Status](https://img.shields.io/jenkins/s/http/vali.inf.ed.ac.uk/jenkins/view/amunmt/job/amun-gpu.svg?label=CUDA%20Build)](http://vali.inf.ed.ac.uk/jenkins/job/amun-gpu/)
[![CPU Tests Status](https://img.shields.io/jenkins/s/http/vali.inf.ed.ac.uk/jenkins/view/amunmt/job/amun-regression-tests-cpu.svg?label=CPU%20Tests)](http://vali.inf.ed.ac.uk/jenkins/job/amun-regression-tests-cpu/)
[![CUDA Tests Status](https://img.shields.io/jenkins/s/http/vali.inf.ed.ac.uk/jenkins/view/amunmt/job/amun-regression-tests-gpu.svg?label=CUDA%20Tests)](http://vali.inf.ed.ac.uk/jenkins/job/amun-regression-tests-gpu/)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE)

Stand-alone C++ decoder for RNN-based NMT models. Can decode with default
models from Marian and Nematus.

This tool was a part of [Marian](https://github.com/marian-nmt/marian), which
now has its own decoder. Therefore, _amun_ has been moved to a separate
repository. For RNN models this decoder can still be faster on the GPU than the
official Marian decoder.

If you use this, please cite:

Marcin Junczys-Dowmunt, Roman Grundkiewicz, Tomasz Dwojak, Hieu Hoang, Kenneth
Heafield, Tom Neckermann, Frank Seide, Ulrich Germann, Alham Fikri Aji, Nikolay
Bogoychev, André F. T. Martins, Alexandra Birch (2018). [Marian: Fast Neural
Machine Translation in C++](http://www.aclweb.org/anthology/P18-4020)

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

other papers about Amun:
	@article{DBLP:journals/corr/Junczys-Dowmunt16c,
	  author    = {Marcin Junczys{-}Dowmunt and
	               Tomasz Dwojak and
	               Hieu Hoang},
	  title     = {Is Neural Machine Translation Ready for Deployment? {A} Case Study
	               on 30 Translation Directions},
	  journal   = {CoRR},
	  volume    = {abs/1610.01108},
	  year      = {2016},
	  url       = {http://arxiv.org/abs/1610.01108},
	  archivePrefix = {arXiv},
	  eprint    = {1610.01108},
	  timestamp = {Mon, 13 Aug 2018 16:48:23 +0200},
	  biburl    = {https://dblp.org/rec/bib/journals/corr/Junczys-Dowmunt16c},
	  bibsource = {dblp computer science bibliography, https://dblp.org}
	}

	@InProceedings{W18-2714,
	  author = 	"Hoang, Hieu
			and Dwojak, Tomasz
			and Krislauks, Rihards
			and Torregrosa, Daniel
			and Heafield, Kenneth",
	  title = 	"Fast Neural Machine Translation Implementation",
	  booktitle = 	"Proceedings of the 2nd Workshop on Neural Machine Translation and Generation",
	  year = 	"2018",
	  publisher = 	"Association for Computational Linguistics",
	  pages = 	"116--121",
	  location = 	"Melbourne, Australia",
	  url = 	"http://aclweb.org/anthology/W18-2714"
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

