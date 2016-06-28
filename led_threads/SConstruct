
#scons file for led using threads

import os
				  
env_options = {
    "CC"    	: "aarch64-linux-gnu-gcc",
    "CCFLAGS" 	: "-Wall -g",
    "LDFLAGS"	: "-pthread",
    "LINKFLAGS"	: "-pthread"
}

env = Environment(**env_options)
env.Append(ENV = {'PATH' : os.environ['PATH']})

env.Program('led_threads', Glob('*.c'))
