
#
# This file is the default set of rules to compile a Pebble project.
#
# Feel free to customize this to your needs.
#

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load("clang_compilation_database", tooldir=".")
    ctx.load('pebble_sdk')

def build(ctx):
    ctx.load('pebble_sdk')

    #Needed for math-sll.h asm functions
    ctx.env.CFLAGS.append('-Wa,-mimplicit-it=always')
    ctx.env.CFLAGS.append('-DFRAMEBUFFER_WIDTH=144')
    ctx.env.CFLAGS.append('-DFRAMEBUFFER_HEIGHT=168')

    ctx.env.CFLAGS.append('-DUSE_FIXED_POINT=1')

    ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'),
                    target='pebble-app.elf')

    ctx.pbl_bundle(elf='pebble-app.elf',
                   js=ctx.path.ant_glob('src/js/**/*.js'))
