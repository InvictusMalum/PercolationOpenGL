project('OpenGL', ['cpp','c'],
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

# PUT glfw3.dll IN THE BUILD DIRECTORY
# USE https://mesonbuild.com/Build-targets.html FOR DOING THIS AUTOMATICALLY

project_dir = meson.current_source_dir()

inc = [include_directories('Libraries/include')]
inc += './Libraries/lib'

cpp = meson.get_compiler('cpp')
c = meson.get_compiler('c')

source_files = ['main.cpp', 'glad.c', 
'glfwClass.cpp',
'shaderClass.cpp',
'EBO.cpp',
'VAO.cpp',
'VBO.cpp']

deps = []

host_system = host_machine.system()
if host_system == 'windows'
  # Undefined references without this one
  deps += cpp.find_library('glfw3dll', dirs : project_dir / 'Libraries/lib', static : true)
  deps += cpp.find_library('glfw3', dirs : project_dir / 'Libraries/lib', static : true)

  # Dynamic is not needed to compile, but is needed in the build
  #deps += cpp.find_library('glfw3', dirs : project_dir / 'Libraries/lib', static : false)
  

elif host_system == 'darwin'
  deps += cpp.find_library('libglfw.3.3', dirs : project_dir + '/Libraries/lib', static : false)

elif host_system == 'linux'
  deps += cpp.find_library('glfw', dirs : project_dir / 'Libraries/lib')
endif

exe = executable('OpenGL', source_files,
    include_directories : inc,
    dependencies : deps)
#    link_with : dyn_glfw.get_('glfw3.dll'))