
#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>

void run_serv(GtkWidget *widget, gpointer data) {
    system("gnome-terminal -- ./serv");      //exec  serv.o et affiche un nv terminal
}

void run_cl(GtkWidget *widget, gpointer data) {
    system("gnome-terminal -- ./cl");    //exec cl.o et affiche un nv terminal
}

void open_guide(GtkWidget *widget, gpointer data) {
    system("xdg-open guide.txt");    // Ouvre le fichier guide.txt 
}

gboolean close_window(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return TRUE;
}

void create_window() {
    GtkWidget *window;
    GtkWidget *fixed;
    GtkWidget *button_box;
    GtkWidget *button_serv;
    GtkWidget *button_cl;
    GtkWidget *button_guide;
    GtkWidget *image_serv;
    GtkWidget *image_cl;
    GtkWidget *label_welcome;
    GtkWidget *image_background;  // Nouveau widget pour l'image d'arrière-plan

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Ma Messagerie");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 350);
    g_signal_connect(window, "delete-event", G_CALLBACK(close_window), NULL);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    // Chargement de l'image d'arrière-plan
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("image.jpg", NULL);
    image_background = gtk_image_new_from_pixbuf(pixbuf);
    gtk_widget_set_size_request(image_background, 600, 400);
    gtk_fixed_put(GTK_FIXED(fixed), image_background, 0, 0);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_fixed_put(GTK_FIXED(fixed), button_box, 0, 300);
    gtk_widget_set_size_request(button_box, 600, 100);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_CENTER);

    button_serv = gtk_button_new_with_label("Démarrer serveur");
    gtk_button_set_relief(GTK_BUTTON(button_serv), GTK_RELIEF_NONE);
    gtk_widget_set_size_request(button_serv, 100, 100);
    g_signal_connect(button_serv, "clicked", G_CALLBACK(run_serv), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button_serv);

    button_cl = gtk_button_new_with_label("Démarrer chat");
    gtk_button_set_relief(GTK_BUTTON(button_cl), GTK_RELIEF_NONE);
    gtk_widget_set_size_request(button_cl, 100, 100);
    g_signal_connect(button_cl, "clicked", G_CALLBACK(run_cl), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button_cl);

    button_guide = gtk_button_new_with_label("Guide");
    gtk_button_set_relief(GTK_BUTTON(button_guide), GTK_RELIEF_NONE);
    gtk_widget_set_size_request(button_guide, 100, 100);
    g_signal_connect(button_guide, "clicked", G_CALLBACK(open_guide), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button_guide);

    image_serv = gtk_image_new_from_file("server_icon.png");
    gtk_button_set_image(GTK_BUTTON(button_serv), image_serv);

    image_cl = gtk_image_new_from_file("chat_icon.png");
    gtk_button_set_image(GTK_BUTTON(button_cl), image_cl);

    label_welcome = gtk_label_new("");
    //label_welcome = gtk_label_new("<span font='Arial Bold 48'>Bienvenue dans l'espace Messenger !</span>");

    gtk_fixed_put(GTK_FIXED(fixed), label_welcome, 0, 0);
    gtk_widget_set_size_request(label_welcome, 600, 200);
    gtk_label_set_justify(GTK_LABEL(label_welcome), GTK_JUSTIFY_CENTER);

    // Ajout d'une décoration de style
    GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "button {"
        "  background-color: #3498db;"
        "  border-radius: 10px;"
        "  margin: 10px;"
        "  transition: background-color 0.3s;"
        "}"
        "button:hover {"
        "  background-color: #2980b9;"
        "}",
        -1, NULL);
    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_widget_show_all(window);

    gtk_main();
}

int main(int argc, char *argv[]) {
    create_window();

    return 0;
}

