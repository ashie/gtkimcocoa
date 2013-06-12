#include <string.h>
#include <gtk/gtk.h>

static GtkWidget *window = NULL;
static GtkWidget *entry = NULL;
static GtkWidget *textview = NULL;
static GtkWidget *treeview = NULL;

static GtkWidget *
create_tree_view (void)
{
    GtkWidget *container;
    GtkListStore *store =
        gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_BOOLEAN);
    GtkTreeIter iter;
    GtkCellRenderer *cell;

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, "Please edit me",
                       1, TRUE,
                       -1);

    container = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    cell = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(container),
                                                0, "column1", cell,
                                                "text", 0,
                                                "editable", 1,
                                                NULL);

    return container;
}

static void
cb_activate(void)
{
    g_print("activate\n");
}

static gboolean
cb_key_release(GtkWidget *widget, GdkEventKey *key_event, gpointer data)
{
    g_print("keyval: %s\n", gdk_keyval_name(key_event->keyval));
    return FALSE;
}

static GtkWidget *
create_window (void)
{
    GtkWidget *window, *vbox;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window),
                                400, 400);

#if (GTK_MAJOR_VERSION >= 3)
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#else
    vbox = gtk_vbox_new(FALSE, 0);
#endif
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX (vbox), entry, FALSE, FALSE, 0);
    g_signal_connect(entry, "activate",
                     G_CALLBACK(cb_activate), NULL);
    g_signal_connect(entry, "key-release-event",
                     G_CALLBACK(cb_key_release), NULL);
    gtk_widget_show(entry);

    textview = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), textview, TRUE, TRUE, 0);
    gtk_widget_show(textview);

    treeview = create_tree_view();
    gtk_box_pack_start(GTK_BOX(vbox), treeview, FALSE, FALSE, 0);
    gtk_widget_show(treeview);

    return window;
}


static void
destroy_window_cb (GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}


int
main (int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    window = create_window();
    gtk_widget_show(window);
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(destroy_window_cb), window);

    gtk_main();

    return 0;
}
