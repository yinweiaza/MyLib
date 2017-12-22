# Install script for directory: F:/study/MyLib/3rdparty

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/MYLIB")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "F:/study/MyLib/build/include/FL/Enumerations.H;F:/study/MyLib/build/include/FL/Fl.H;F:/study/MyLib/build/include/FL/Fl_Adjuster.H;F:/study/MyLib/build/include/FL/Fl_BMP_Image.H;F:/study/MyLib/build/include/FL/Fl_Bitmap.H;F:/study/MyLib/build/include/FL/Fl_Box.H;F:/study/MyLib/build/include/FL/Fl_Browser.H;F:/study/MyLib/build/include/FL/Fl_Browser_.H;F:/study/MyLib/build/include/FL/Fl_Button.H;F:/study/MyLib/build/include/FL/Fl_Cairo.H;F:/study/MyLib/build/include/FL/Fl_Cairo_Window.H;F:/study/MyLib/build/include/FL/Fl_Chart.H;F:/study/MyLib/build/include/FL/Fl_Check_Browser.H;F:/study/MyLib/build/include/FL/Fl_Check_Button.H;F:/study/MyLib/build/include/FL/Fl_Choice.H;F:/study/MyLib/build/include/FL/Fl_Clock.H;F:/study/MyLib/build/include/FL/Fl_Color_Chooser.H;F:/study/MyLib/build/include/FL/Fl_Copy_Surface.H;F:/study/MyLib/build/include/FL/Fl_Counter.H;F:/study/MyLib/build/include/FL/Fl_Device.H;F:/study/MyLib/build/include/FL/Fl_Dial.H;F:/study/MyLib/build/include/FL/Fl_Double_Window.H;F:/study/MyLib/build/include/FL/Fl_Export.H;F:/study/MyLib/build/include/FL/Fl_File_Browser.H;F:/study/MyLib/build/include/FL/Fl_File_Chooser.H;F:/study/MyLib/build/include/FL/Fl_File_Icon.H;F:/study/MyLib/build/include/FL/Fl_File_Input.H;F:/study/MyLib/build/include/FL/Fl_Fill_Dial.H;F:/study/MyLib/build/include/FL/Fl_Fill_Slider.H;F:/study/MyLib/build/include/FL/Fl_Float_Input.H;F:/study/MyLib/build/include/FL/Fl_FormsBitmap.H;F:/study/MyLib/build/include/FL/Fl_FormsPixmap.H;F:/study/MyLib/build/include/FL/Fl_Free.H;F:/study/MyLib/build/include/FL/Fl_GIF_Image.H;F:/study/MyLib/build/include/FL/Fl_Gl_Window.H;F:/study/MyLib/build/include/FL/Fl_Group.H;F:/study/MyLib/build/include/FL/Fl_Help_Dialog.H;F:/study/MyLib/build/include/FL/Fl_Help_View.H;F:/study/MyLib/build/include/FL/Fl_Hold_Browser.H;F:/study/MyLib/build/include/FL/Fl_Hor_Fill_Slider.H;F:/study/MyLib/build/include/FL/Fl_Hor_Nice_Slider.H;F:/study/MyLib/build/include/FL/Fl_Hor_Slider.H;F:/study/MyLib/build/include/FL/Fl_Hor_Value_Slider.H;F:/study/MyLib/build/include/FL/Fl_Image.H;F:/study/MyLib/build/include/FL/Fl_Image_Surface.H;F:/study/MyLib/build/include/FL/Fl_Input.H;F:/study/MyLib/build/include/FL/Fl_Input_.H;F:/study/MyLib/build/include/FL/Fl_Input_Choice.H;F:/study/MyLib/build/include/FL/Fl_Int_Input.H;F:/study/MyLib/build/include/FL/Fl_JPEG_Image.H;F:/study/MyLib/build/include/FL/Fl_Light_Button.H;F:/study/MyLib/build/include/FL/Fl_Line_Dial.H;F:/study/MyLib/build/include/FL/Fl_Menu.H;F:/study/MyLib/build/include/FL/Fl_Menu_.H;F:/study/MyLib/build/include/FL/Fl_Menu_Bar.H;F:/study/MyLib/build/include/FL/Fl_Menu_Button.H;F:/study/MyLib/build/include/FL/Fl_Menu_Item.H;F:/study/MyLib/build/include/FL/Fl_Menu_Window.H;F:/study/MyLib/build/include/FL/Fl_Multi_Browser.H;F:/study/MyLib/build/include/FL/Fl_Multi_Label.H;F:/study/MyLib/build/include/FL/Fl_Multiline_Input.H;F:/study/MyLib/build/include/FL/Fl_Multiline_Output.H;F:/study/MyLib/build/include/FL/Fl_Native_File_Chooser.H;F:/study/MyLib/build/include/FL/Fl_Nice_Slider.H;F:/study/MyLib/build/include/FL/Fl_Object.H;F:/study/MyLib/build/include/FL/Fl_Output.H;F:/study/MyLib/build/include/FL/Fl_Overlay_Window.H;F:/study/MyLib/build/include/FL/Fl_PNG_Image.H;F:/study/MyLib/build/include/FL/Fl_PNM_Image.H;F:/study/MyLib/build/include/FL/Fl_Pack.H;F:/study/MyLib/build/include/FL/Fl_Paged_Device.H;F:/study/MyLib/build/include/FL/Fl_Pixmap.H;F:/study/MyLib/build/include/FL/Fl_Plugin.H;F:/study/MyLib/build/include/FL/Fl_Positioner.H;F:/study/MyLib/build/include/FL/Fl_PostScript.H;F:/study/MyLib/build/include/FL/Fl_Preferences.H;F:/study/MyLib/build/include/FL/Fl_Printer.H;F:/study/MyLib/build/include/FL/Fl_Progress.H;F:/study/MyLib/build/include/FL/Fl_RGB_Image.H;F:/study/MyLib/build/include/FL/Fl_Radio_Button.H;F:/study/MyLib/build/include/FL/Fl_Radio_Light_Button.H;F:/study/MyLib/build/include/FL/Fl_Radio_Round_Button.H;F:/study/MyLib/build/include/FL/Fl_Repeat_Button.H;F:/study/MyLib/build/include/FL/Fl_Return_Button.H;F:/study/MyLib/build/include/FL/Fl_Roller.H;F:/study/MyLib/build/include/FL/Fl_Round_Button.H;F:/study/MyLib/build/include/FL/Fl_Round_Clock.H;F:/study/MyLib/build/include/FL/Fl_Scroll.H;F:/study/MyLib/build/include/FL/Fl_Scrollbar.H;F:/study/MyLib/build/include/FL/Fl_Secret_Input.H;F:/study/MyLib/build/include/FL/Fl_Select_Browser.H;F:/study/MyLib/build/include/FL/Fl_Shared_Image.H;F:/study/MyLib/build/include/FL/Fl_Simple_Counter.H;F:/study/MyLib/build/include/FL/Fl_Single_Window.H;F:/study/MyLib/build/include/FL/Fl_Slider.H;F:/study/MyLib/build/include/FL/Fl_Spinner.H;F:/study/MyLib/build/include/FL/Fl_Sys_Menu_Bar.H;F:/study/MyLib/build/include/FL/Fl_Table.H;F:/study/MyLib/build/include/FL/Fl_Table_Row.H;F:/study/MyLib/build/include/FL/Fl_Tabs.H;F:/study/MyLib/build/include/FL/Fl_Text_Buffer.H;F:/study/MyLib/build/include/FL/Fl_Text_Display.H;F:/study/MyLib/build/include/FL/Fl_Text_Editor.H;F:/study/MyLib/build/include/FL/Fl_Tile.H;F:/study/MyLib/build/include/FL/Fl_Tiled_Image.H;F:/study/MyLib/build/include/FL/Fl_Timer.H;F:/study/MyLib/build/include/FL/Fl_Toggle_Button.H;F:/study/MyLib/build/include/FL/Fl_Toggle_Light_Button.H;F:/study/MyLib/build/include/FL/Fl_Toggle_Round_Button.H;F:/study/MyLib/build/include/FL/Fl_Tooltip.H;F:/study/MyLib/build/include/FL/Fl_Tree.H;F:/study/MyLib/build/include/FL/Fl_Tree_Item.H;F:/study/MyLib/build/include/FL/Fl_Tree_Item_Array.H;F:/study/MyLib/build/include/FL/Fl_Tree_Prefs.H;F:/study/MyLib/build/include/FL/Fl_Valuator.H;F:/study/MyLib/build/include/FL/Fl_Value_Input.H;F:/study/MyLib/build/include/FL/Fl_Value_Output.H;F:/study/MyLib/build/include/FL/Fl_Value_Slider.H;F:/study/MyLib/build/include/FL/Fl_Widget.H;F:/study/MyLib/build/include/FL/Fl_Window.H;F:/study/MyLib/build/include/FL/Fl_Wizard.H;F:/study/MyLib/build/include/FL/Fl_XBM_Image.H;F:/study/MyLib/build/include/FL/Fl_XPM_Image.H;F:/study/MyLib/build/include/FL/abi-version.h;F:/study/MyLib/build/include/FL/dirent.h;F:/study/MyLib/build/include/FL/filename.H;F:/study/MyLib/build/include/FL/fl_ask.H;F:/study/MyLib/build/include/FL/fl_draw.H;F:/study/MyLib/build/include/FL/fl_message.H;F:/study/MyLib/build/include/FL/fl_show_colormap.H;F:/study/MyLib/build/include/FL/fl_show_input.H;F:/study/MyLib/build/include/FL/fl_types.h;F:/study/MyLib/build/include/FL/fl_utf8.h;F:/study/MyLib/build/include/FL/forms.H;F:/study/MyLib/build/include/FL/gl.h;F:/study/MyLib/build/include/FL/gl2opengl.h;F:/study/MyLib/build/include/FL/gl_draw.H;F:/study/MyLib/build/include/FL/glu.h;F:/study/MyLib/build/include/FL/glut.H;F:/study/MyLib/build/include/FL/mac.H;F:/study/MyLib/build/include/FL/math.h;F:/study/MyLib/build/include/FL/names.h;F:/study/MyLib/build/include/FL/win32.H;F:/study/MyLib/build/include/FL/x.H")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/include/FL" TYPE FILE FILES
    "F:/study/MyLib/3rdparty/include/fltk/FL/Enumerations.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Adjuster.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_BMP_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Bitmap.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Box.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Browser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Browser_.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Cairo.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Cairo_Window.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Chart.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Check_Browser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Check_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Choice.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Clock.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Color_Chooser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Copy_Surface.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Counter.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Device.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Dial.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Double_Window.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Export.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_File_Browser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_File_Chooser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_File_Icon.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_File_Input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Fill_Dial.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Fill_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Float_Input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_FormsBitmap.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_FormsPixmap.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Free.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_GIF_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Gl_Window.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Group.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Help_Dialog.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Help_View.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Hold_Browser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Hor_Fill_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Hor_Nice_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Hor_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Hor_Value_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Image_Surface.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Input_.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Input_Choice.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Int_Input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_JPEG_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Light_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Line_Dial.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Menu.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Menu_.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Menu_Bar.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Menu_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Menu_Item.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Menu_Window.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Multi_Browser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Multi_Label.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Multiline_Input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Multiline_Output.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Native_File_Chooser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Nice_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Object.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Output.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Overlay_Window.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_PNG_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_PNM_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Pack.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Paged_Device.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Pixmap.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Plugin.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Positioner.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_PostScript.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Preferences.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Printer.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Progress.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_RGB_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Radio_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Radio_Light_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Radio_Round_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Repeat_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Return_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Roller.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Round_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Round_Clock.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Scroll.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Scrollbar.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Secret_Input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Select_Browser.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Shared_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Simple_Counter.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Single_Window.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Spinner.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Sys_Menu_Bar.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Table.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Table_Row.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tabs.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Text_Buffer.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Text_Display.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Text_Editor.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tile.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tiled_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Timer.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Toggle_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Toggle_Light_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Toggle_Round_Button.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tooltip.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tree.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tree_Item.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tree_Item_Array.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Tree_Prefs.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Valuator.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Value_Input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Value_Output.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Value_Slider.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Widget.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Window.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_Wizard.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_XBM_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/Fl_XPM_Image.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/abi-version.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/dirent.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/filename.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/fl_ask.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/fl_draw.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/fl_message.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/fl_show_colormap.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/fl_show_input.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/fl_types.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/fl_utf8.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/forms.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/gl.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/gl2opengl.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/gl_draw.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/glu.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/glut.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/mac.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/math.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/names.h"
    "F:/study/MyLib/3rdparty/include/fltk/FL/win32.H"
    "F:/study/MyLib/3rdparty/include/fltk/FL/x.H"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "F:/study/MyLib/build/lib/fltk_formsd.lib;F:/study/MyLib/build/lib/fltk_gld.lib;F:/study/MyLib/build/lib/fltk_imagesd.lib;F:/study/MyLib/build/lib/fltk_jpegd.lib;F:/study/MyLib/build/lib/fltk_pngd.lib;F:/study/MyLib/build/lib/fltk_zd.lib;F:/study/MyLib/build/lib/fltkd.lib")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/lib" TYPE FILE FILES
    "F:/study/MyLib/3rdparty/lib/fltk/fltk_formsd.lib"
    "F:/study/MyLib/3rdparty/lib/fltk/fltk_gld.lib"
    "F:/study/MyLib/3rdparty/lib/fltk/fltk_imagesd.lib"
    "F:/study/MyLib/3rdparty/lib/fltk/fltk_jpegd.lib"
    "F:/study/MyLib/3rdparty/lib/fltk/fltk_pngd.lib"
    "F:/study/MyLib/3rdparty/lib/fltk/fltk_zd.lib"
    "F:/study/MyLib/3rdparty/lib/fltk/fltkd.lib"
    )
endif()

