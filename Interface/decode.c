#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//# include "../Encoding/headers/enc_main.h"
# include "../segmentation/segmentation.h"
# include "../Encoding/headers/decode.h"

GtkBuilder* builder; 
GtkWidget* window;
GtkWidget* output;

GtkWidget* dialog;

GtkWidget* output;
GtkWidget* qrcode;

char *filename;

int demo = 0;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/decoding.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    gtk_builder_connect_signals(builder, NULL);

    qrcode = GTK_WIDGET(gtk_builder_get_object(builder, "qrcode"));
    output = GTK_WIDGET(gtk_builder_get_object(builder, "output"));
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "SET_FILE"));

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void changed_file()
{
}

void cancel_clicked_cb()
{
}

void destroy()
{
    gtk_main_quit();
}

void demo_act()
{
    demo = demo == 0 ? 1 : 0;
    printf("%i\n", demo);
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

        gtk_entry_set_text(GTK_ENTRY(output), string);
    }
}

void file_set()
{
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window), action,
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

        gtk_image_set_from_file(GTK_IMAGE(qrcode), filename);

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

    gtk_widget_destroy(dialog);

}
