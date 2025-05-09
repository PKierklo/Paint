add_executable(EditImageApp
    ColorPickerDialog.cpp
    ColorPickerDialog.h
    DrawingApp.cpp
    DrawingCanvas.cpp
    DrawingCanvas.h
    DrawingModel.cpp
    DrawingModel.h
    GeometryUtils.cpp
    GeometryUtils.h
    IDrawingModel.h
    Main.cpp
    MainFrame.cpp
    MainFrame.h
    RGBCube.cpp
    RGBCube.h
)

find_package(wxWidgets REQUIRED)
target_include_directories(EditImageApp PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(EditImageApp PRIVATE ${wxWidgets_LIBRARIES})
