#include <gtk/gtk.h>
#include <string.h>

# include "../Encoding/headers/enc_main.h"

GtkBuilder* builder; 
GtkWidget* window;
GtkWidget* output;

GtkWidget* input;
GtkWidget* out_path;
GtkWidget* adv_opt;

GtkWidget* level;
GtkWidget* vers;
GtkWidget* mask;

GtkWidget* output;
GtkWidget* qrcode;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/epicode.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    output = GTK_WIDGET(gtk_builder_get_object(builder, "output"));

    gtk_builder_connect_signals(builder, NULL);

    input = GTK_WIDGET(gtk_builder_get_object(builder, "input"));
    out_path = GTK_WIDGET(gtk_builder_get_object(builder, "out_path"));
    adv_opt = GTK_WIDGET(gtk_builder_get_object(builder, "adv_opts"));


    level = GTK_WIDGET(gtk_builder_get_object(builder, "level"));
    vers = GTK_WIDGET(gtk_builder_get_object(builder, "version"));
    mask = GTK_WIDGET(gtk_builder_get_object(builder, "mask"));

    qrcode = GTK_WIDGET(gtk_builder_get_object(builder, "qrCode"));

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

void on_adv_opts_toggled()
{
    gtk_widget_set_sensitive(level, !gtk_widget_get_sensitive(level));
    gtk_widget_set_sensitive(vers, !gtk_widget_get_sensitive(vers));
    gtk_widget_set_sensitive(mask, !gtk_widget_get_sensitive(mask));
}

void on_button1_clicked()
{
    gint c_level = 0;
    gint c_vers = 41;
    //gint c_mask = 8;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(adv_opt)))
    {
        // if the advanced options are enabled then we need to retrieve them
        c_level = gtk_combo_box_get_active(GTK_COMBO_BOX(level));
        c_vers = gtk_combo_box_get_active(GTK_COMBO_BOX(vers)) + 1;
        //c_mask = gtk_combo_box_get_active(GTK_COMBO_BOX(mask));
    }

    char* s_level;
    switch(c_level)
    {
        case 1:
            s_level = "M";
            break;
        case 2:
            s_level = "Q";
            break;
        case 3:
            s_level = "H";
            break;
        default:
            s_level = "L";
            break;

    }
    char s_vers[4];
    sprintf(s_vers, "%d", c_vers);

    char* input_txt;

    input_txt = gtk_entry_get_text(GTK_ENTRY(input));
    if(strlen(input_txt) == 0)
        printf("Entry empty.\n");
    else
    {
        char* cmd[7];
        cmd[0] = "./enc_main";
        cmd[1] = "-d";
        cmd[2] = input_txt;
        cmd[3] = "-c";
        cmd[4] = s_level;
        cmd[5] = "-v";
        cmd[6] = s_vers;
        int argc = 7;

        enc_main(argc , cmd);
        gtk_widget_show(output);
        gtk_image_set_from_file(GTK_IMAGE(qrcode), "./output/test.bmp");
    }
}

void on_button2_clicked()
{
    gtk_entry_set_text(GTK_ENTRY(input), "");
}
