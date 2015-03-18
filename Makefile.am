##############################################################################
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
# 
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
# 
##############################################################################

SUBDIRS = @MAKE_SUB_DIRS@

bin_SCRIPTS    = jhpcndf-config
CLEANFILES     = $(bin_SCRIPTS)
EXTRA_DIST     = jhpcndf-config.in LICENSE src/Doxyfile jhpcndf-uname
dist_data_DATA = AUTHORS COPYING ChangeLog README LICENSE

clean-local:
	find ./ -name *.obj | xargs rm -f
	find ./ -name *.exe | xargs rm -f
	find ./ -name *.lib | xargs rm -f

distclean-local: clean-local
	rm -f ./bin/*
	rm -f ./lib/*

