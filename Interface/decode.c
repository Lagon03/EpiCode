#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//# include "../Encoding/headers/enc_main.h"

GtkBuilder* builder; 
GtkWidget* window;
GtkWidget* output;

GtkWidget* dialog;

GtkWidget* output;
GtkWidget* qrcode;

char *filename;

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

void decod_clicked()
{
    //char* string = "Not implemented yet.";
    gtk_entry_set_text(GTK_ENTRY(output), filename);
}

void file_set()
{
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
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
