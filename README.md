# SOFT356-Reece-Drage
## Project Aims:
The project aims to allow for the parsing and rendering of primitive 3D model data from OBJ and MTL files, displaying in a window with limited user interaction.
The user will be able to input a file to load, as well as rotate the model and clear the display.
## Software and Libraries Used:
The software was written using Visual Studio Community 2019 version 16.3.7.
The NuGet packages, NupenGL.core version 0.1.0.1 and NupenGL.core.redist version 0.1.0.1 were installed for the usage of GLEW, FreeGLUT and GLFW.
The NuGet package GLM version 0.9.9.600 was used for mathematic operations involving matrices and vectors, as well as efficient interactions between C++ data types and GLSL data types. 
## Project Navigation and Design:
Multiple classes have been used for the development of this project. 
Model loading and parsing is performed by the ModelLoader class in ModelLoader.cpp. 
This class is responsible for reading an OBJ file line-by-line, then interpreting and converting vertex data, texture coordinate data and normal data into usable values. 
As well as this, the class also reads and parses MTL files to generate material data for rendering colours, lighting and textures. 
The data from OBJ and MTL files is used to generate Model structs, which contain a vector of ModelObject structs, themselves containing vectors of VertexGroup structs. 
The VertexGroup structs contain vectors of vertex coordinates, texture coordinates and normal coordinates, as well as a corresponding material data, held in a MaterialData struct.
Rendering is performed by interactions between 2 classes. 
The OpenGLHandler class in OpenGLHandler.cpp initialises GLFW and GLEW and create a display window. 
The RenderableModel class in RenderableModel.cpp calls the ModelLoader functionality to load model data, initialises a VAO and corresponding buffers and can then draw to the output window.
The Viewport class in Viewport.cpp handles a model x view x projection matrix, and is used to contain functionality related to scaling, rotating and manipulating models and the view.  
## Completed Model Loader Functionality:
The model loader can currently load OBJ files with MTL files containing material data.
The model contains a full set of vertex data, texture coordinates and normal data.
The material data consists of ambient, diffuse and specular lighting colours, as well as a texture file.
Currently the loaded model displays with wrapped textures and ambient and diffuse lighting.
User input can rotate the model to the left or right.
Multiple models can be displayed simultaneously, though they share a single MVP, so any rotations affect all models simultaneously.
Models have an offset, though rendering large numbers of models will extend them outside of the view at some angles.
No file formats outside of OBJ files and MTL files are usable, currently.
## Using the Model Loader:
- Upon running the file, the user is prompted to enter a model file path. Test files are located in “Models/”.
- At run-time, press “A” or “D” to rotate models left and right, respectively. The command prompt window must be selected for this to work.
- Press “P” to pause the render, then enter “LOAD” to load another model file or “CLEAR” to remove all models from the render window. The command prompt window must be selected for this to work.


