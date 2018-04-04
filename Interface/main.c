#include <gtk/gtk.h>
#include <string.h>
GtkWidget* input;
GtkWidget* out_path;

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/epicode.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    gtk_builder_connect_signals(builder, NULL);

    input = GTK_WIDGET(gtk_builder_get_object(builder, "input"));
    out_path = GTK_WIDGET(gtk_builder_get_object(builder, "out_path"));
 
    g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}
 
// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_button1_clicked()
{
    const char* input_txt;

    input_txt = gtk_entry_get_text(GTK_ENTRY(input));
    if(strlen(input_txt) == 0)
        printf("Entry empty.\n");
    else
        printf("%s\n", input_txt);
}

void on_button2_clicked()
{
    gtk_entry_set_text(GTK_ENTRY(input), "t");
}
