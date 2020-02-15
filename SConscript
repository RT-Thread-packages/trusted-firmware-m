# RT-Thread building script for component

from building import *

cwd = GetCurrentDir()
src = ['interface/src/tfm_sst_api.c']
src += ['interface/src/tfm_ns_lock_rt-thread.c']
src += ['cmake_build/install/export/tfm/veneers/s_veneers.o']
CPPPATH = [cwd + '/interface/include']

group = DefineGroup('TFM', src, depend = ['PKG_USING_TFM'], CPPPATH = CPPPATH)

Return('group')
