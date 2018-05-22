#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>

# include "../Encoding/headers/enc_main.h"
# include "../segmentation/segmentation.h"
# include "../Encoding/headers/decode.h"


GtkBuilder* builder;
GtkWidget* epicode;
GtkWidget* epidec;
GtkWidget* output;

GtkWidget* input;
GtkWidget* out_path;
GtkWidget* adv_opt;

GtkWidget* level;
GtkWidget* vers;
GtkWidget* mask;
GtkWidget* epi;

GtkWidget* output;
GtkWidget* qrcode;
GtkWidget* output_d;
GtkWidget* qrcode_d;

GtkWidget* dialog;

char *filename;
char *saveAs;

int demo = 0;
int epic = 0;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/multi.glade", NULL);

    epicode = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    epidec = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    output = GTK_WIDGET(gtk_builder_get_object(builder, "output"));
    output_d = GTK_WIDGET(gtk_builder_get_object(builder, "output1"));

    gtk_builder_connect_signals(builder, NULL);

    input = GTK_WIDGET(gtk_builder_get_object(builder, "input"));
    out_path = GTK_WIDGET(gtk_builder_get_object(builder, "out_path"));
    adv_opt = GTK_WIDGET(gtk_builder_get_object(builder, "adv_opts"));


    level = GTK_WIDGET(gtk_builder_get_object(builder, "level"));
    vers = GTK_WIDGET(gtk_builder_get_object(builder, "version"));
    mask = GTK_WIDGET(gtk_builder_get_object(builder, "mask"));
    epi = GTK_WIDGET(gtk_builder_get_object(builder, "epic"));


    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "SET_FILE"));


    qrcode = GTK_WIDGET(gtk_builder_get_object(builder, "qrCode"));
    qrcode_d = GTK_WIDGET(gtk_builder_get_object(builder, "qrcode_d"));

    g_object_unref(builder);

    gtk_widget_show(epicode);
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

void switch_window() {
    if(gtk_widget_get_visible(epicode)) {
        gtk_widget_hide(epicode);
        gtk_widget_show(epidec);
    }
    else {
        gtk_widget_show(epicode);
        gtk_widget_hide(epidec);
    }
}

void on_adv_opts_toggled()
{
    gtk_widget_set_sensitive(level, !gtk_widget_get_sensitive(level));
    gtk_widget_set_sensitive(vers, !gtk_widget_get_sensitive(vers));
    gtk_widget_set_sensitive(mask, !gtk_widget_get_sensitive(mask));
}

void on_button1_clicked()
{
    printf("path : %s\n", saveAs);
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

    gint epic = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(epi));
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
    char s_vers[12];
    sprintf(s_vers, "%d", c_vers);

    const char* input_txt;

    input_txt = gtk_entry_get_text(GTK_ENTRY(input));
    if(strlen(input_txt) == 0)
        printf("Entry empty.\n");
    else
    {
        char* cmd[11];
        int argc = 1;
        cmd[0] = "./enc_main";
        cmd[1] = "-x";
        cmd[2] = "-d";
        cmd[3] = (char*)input_txt;
        cmd[4] = "-c";
        cmd[5] = s_level;
        cmd[6] = "-v";
        cmd[7] = s_vers;
        argc = 8;
        if(epic == 1 && saveAs) {
            cmd[8] = "-o";
            cmd[9] = saveAs;
            cmd[10] = "-t";
            argc = 11;
        }
        else if(epic) {
            cmd[8] = "-t";
            argc = 9;
        }
        else if(saveAs) {
            cmd[8] = "-o";
            cmd[9] = saveAs;
            argc = 10;
        }

        enc_main(argc, cmd);
        gtk_widget_show(output);
        gtk_image_set_from_file(GTK_IMAGE(qrcode), saveAs);

        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(qrcode));
        if (pixbuf == NULL)
        {
            g_printerr("Failed to resize image\n");
        }
        else
        {
            pixbuf = gdk_pixbuf_scale_simple(pixbuf, gtk_widget_get_allocated_width(qrcode), gtk_widget_get_allocated_height(qrcode), GDK_INTERP_BILINEAR);
            gtk_image_set_from_pixbuf(GTK_IMAGE(qrcode), pixbuf);
        }
    }
}

void on_button2_clicked()
{
    gtk_entry_set_text(GTK_ENTRY(input), "");
}

void demo_act()
{
    demo = demo == 0 ? 1 : 0;
    //printf("%i\n", demo);
}

void epic_act()
{
    epic = epic == 0 ? 1 : 0;
    //printf("%i\n", demo);
}

void decod_clicked()
{
    if(!filename)
        printf("No decoding file.");
    else
    {
        struct PCode* data;
        if (demo == 1)
            data = SegmentationFromFile(filename, 1);
        else if (epic == 1)
            data = SegmentationFromFile(filename, 2);
        else
            data = SegmentationFromFile(filename, 0);
        int s_level;
        switch(data->ECL)
        {
            case 'M':
                s_level = 1;
                break;
            case 'Q':
                s_level = 2;
                break;
            case 'H':
                s_level = 3;
                break;
            default:
                s_level = 0;
                break;
        }
        char* string = "Not implemented yet.";
        string = decode(data->BStream, data->Version, s_level);

        gtk_entry_set_text(GTK_ENTRY(output_d), string);
    }
}

void file_set()
{
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(epidec), action,
            ("_Cancel"),
            GTK_RESPONSE_CANCEL,
            ("_Open"),
            GTK_RESPONSE_ACCEPT,
            NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if(res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        gtk_widget_destroy(dialog);

        gtk_image_set_from_file(GTK_IMAGE(qrcode_d), filename);

        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(qrcode_d));
        if (pixbuf == NULL)
        {
            g_printerr("Failed to resize image\n");
        }
        else
        {
            pixbuf = gdk_pixbuf_scale_simple(pixbuf, gtk_widget_get_allocated_width(qrcode_d), gtk_widget_get_allocated_height(qrcode_d), GDK_INTERP_BILINEAR);
            gtk_image_set_from_pixbuf(GTK_IMAGE(qrcode_d), pixbuf);
        }
    }

    gtk_widget_destroy(dialog);

}

void changed_file() {}
void cancel_clicked_cb() {}

void quit_output() {
    gtk_widget_hide(output);
}

void save_file() {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Save File",
            GTK_WINDOW(output),
            action,
            ("_Cancel"),
            GTK_RESPONSE_CANCEL,
            ("_Save"),
            GTK_RESPONSE_ACCEPT,
            NULL);
    chooser = GTK_FILE_CHOOSER (dialog);

    gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

    gtk_file_chooser_set_filename (chooser,
            "output/test.bmp");

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (chooser);
        saveAs = filename;
        //g_free (filename);
    }

    gtk_widget_destroy (dialog);
}
