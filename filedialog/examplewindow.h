#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();
  virtual ~ExampleWindow();
  std::string GetFilename();

protected:
  //Signal handlers:
  void on_button_file_clicked();
  void on_button_folder_clicked();

  //Child widgets:
  Gtk::VButtonBox m_ButtonBox;
  Gtk::Button m_Button_File, m_Button_Folder;

  
  std::string m_filename;
};

#endif //GTKMM_EXAMPLEWINDOW_H