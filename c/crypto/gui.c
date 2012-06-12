#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include "des.h"
#include "rsa.h"
#include "sha.h"

/* file path for File dialog */
char *file_path;

/* GUI main widgets */
GtkWidget *window, *hbox, *vbox, *menu_bar, *notebook;

/* menu widgets */
GtkWidget *file_menu, *file_item, *quit_item;
GtkWidget *edit_menu, *edit_item, *cut_item, *copy_item, *paste_item;
GtkWidget *help_menu, *help_item, *about_item;
	
/* Frame widgets */
GtkWidget *frame_symetric, *frame_asymetric, *frame_hash, *label;
GtkWidget *frame_envelope, *frame_signature, *frame_stamp;

/* "Envelope" frame widgets */
GtkWidget *env_in_path, *env_in_select, *env_in_view;
GtkWidget *env_out_path, *env_out_select, *env_out_view;
GtkWidget *env_envelope, *env_select, *env_view;
GtkWidget *env_pub_key_path, *env_pub_key_select, *env_pub_key_view;
GtkWidget *env_prv_key_path, *env_prv_key_select, *env_prv_key_view;
GtkWidget *env_open, *env_generate;

/* "Signature" frame widgets */ 
GtkWidget *sgn_in_path, *sgn_in_select, *sgn_in_view;
GtkWidget *sgn_out_path, *sgn_out_select, *sgn_out_view;
GtkWidget *sgn_pub_key_path, *sgn_pub_key_select, *sgn_pub_key_view;
GtkWidget *sgn_prv_key_path, *sgn_prv_key_select, *sgn_prv_key_view;
GtkWidget *sgn_signature, *sgn_select, *sgn_view;
GtkWidget *sgn_generate, *sgn_check;

/* "Stamp" frame widgets */
GtkWidget *stmp_in_path, *stmp_in_select, *stmp_in_view;
GtkWidget *stmp_out_path, *stmp_out_select, *stmp_out_view;
GtkWidget *stmp_snd_prv_key_path, *stmp_snd_prv_key_select, *stmp_snd_prv_key_view;
GtkWidget *stmp_snd_pub_key_path, *stmp_snd_pub_key_select, *stmp_snd_pub_key_view;
GtkWidget *stmp_rcv_prv_key_path, *stmp_rcv_prv_key_select, *stmp_rcv_prv_key_view;
GtkWidget *stmp_rcv_pub_key_path, *stmp_rcv_pub_key_select, *stmp_rcv_pub_key_view;
GtkWidget *stmp_sgn_path, *stmp_sgn_select, *stmp_sgn_view;
GtkWidget *stmp_env_path, *stmp_env_select, *stmp_env_view;
GtkWidget *stmp_generate, *stmp_open;

/* "Symetric" frame widgets */
GtkWidget *sym_key_path, *sym_key_select, *sym_key_view, *sym_key_generate;
GtkWidget *sym_in_path, *sym_in_select, *sym_in_view;
GtkWidget *sym_out_path, *sym_out_select, *sym_out_view;
GtkWidget *sym_encrypt, *sym_decrypt;

/* "Asymetric" frame widgets */
GtkWidget *asym_pub_key_path, *asym_pub_key_select, *asym_pub_key_view, *asym_pub_key_generate;
GtkWidget *asym_prv_key_path, *asym_prv_key_select, *asym_prv_key_view, *asym_prv_key_generate;
GtkWidget *asym_in_path, *asym_in_select, *asym_in_view;
GtkWidget *asym_out_path, *asym_out_select, *asym_out_view;
GtkWidget *asym_key_length, *asym_encrypt, *asym_decrypt;

/* "Hash" frame widgets */
GtkWidget *hsh_in_path, *hsh_in_select, *hsh_in_view;
GtkWidget *hsh_out_path, *hsh_out_select, *hsh_out_view;
GtkWidget *hsh_calculate, *hsh_hash;


/* "View" file */
static void view_file (GtkWidget *widget, gpointer *filename)
{
	pid_t pid = fork();
		
	if (pid == -1) {
		perror ("fork");
	} else if (!pid) {
		char *editor = getenv ("EDITOR");
		char *file = GTK_ENTRY (filename)->text;
		char *arg_list[] = {editor, file, NULL};

		execvp (editor, arg_list);
		_exit (0);
	}
}

/* About dialog */
static void show_credits()
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "(C) 2005 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

/* sets filename when "OK" is clicked in File dialog */
static void file_return_path (GtkWidget *widget, GtkFileSelection *fs)
{
	file_path = (char *) gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs));
}

/* sets filename to text box */
static void file_set_path (GtkWidget *widget)
{
	gtk_entry_set_text (GTK_ENTRY (widget), file_path);
}

/* file select dialog */
static void file_select (GtkWidget *widget, gpointer *data)
{
	GtkWidget *dialog;
	
	dialog = gtk_file_selection_new ("Select file");
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (dialog)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (dialog));
	g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button), "clicked", G_CALLBACK (file_return_path), dialog);
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button), "clicked", G_CALLBACK (file_set_path), data);
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (dialog));
	gtk_widget_show (dialog);
}

static void hsh_callback()
{
	char *output = GTK_ENTRY (hsh_out_path)->text;
	char *hash = sha1 (GTK_ENTRY (hsh_in_path)->text, output);
	
	gtk_entry_set_text (GTK_ENTRY (hsh_hash), hash); 
}

static void sym_encrypt_callback()
{
	char *in_file = GTK_ENTRY (sym_in_path)->text;
	char *out_file = GTK_ENTRY (sym_out_path)->text;
	char *key_file = GTK_ENTRY (sym_key_path)->text;

	char *tmp_out_file = "des_out_XXXXXX.$$$";
	
	des_crypt (in_file, tmp_out_file, des_get_key_from_file (key_file), DES_ENCRYPT);
	des_convert_to_base64 (tmp_out_file, out_file);

	unlink (tmp_out_file);
}

static void sym_decrypt_callback()
{
	char *in_file = GTK_ENTRY (sym_in_path)->text;
	char *out_file = GTK_ENTRY (sym_out_path)->text;
	char *key_file = GTK_ENTRY (sym_key_path)->text;

	char *tmp_out_file = "des_out_XXXXXX.$$$";
	
	des_convert_from_base64 (in_file, tmp_out_file);
	des_crypt (tmp_out_file, out_file, des_get_key_from_file (key_file), DES_DECRYPT);

	unlink (tmp_out_file);
}

static void sym_key_generate_callback()
{
	des_generate_key (GTK_ENTRY (sym_key_path)->text);
}

static void asym_encrypt_callback()
{
	char *in_file = GTK_ENTRY (asym_in_path)->text;
	char *out_file = GTK_ENTRY (asym_out_path)->text;
	char *key_file = GTK_ENTRY (asym_pub_key_path)->text;
	const char *tmp_out_file = "/tmp/rsa.out.tmp";
	
	rsa_crypt (key_file, in_file, tmp_out_file, RSA_ENCRYPT);
	rsa_convert_to_base64 (tmp_out_file, out_file);

	unlink (tmp_out_file);
}

static void asym_decrypt_callback()
{
	char *in_file = GTK_ENTRY (asym_in_path)->text;
	char *out_file = GTK_ENTRY (asym_out_path)->text;
	char *key_file = GTK_ENTRY (asym_prv_key_path)->text;
	const char *tmp_out_file = "/tmp/rsa.out.tmp";
	
	rsa_convert_from_base64 (in_file, tmp_out_file);
	rsa_crypt (key_file, tmp_out_file, out_file, RSA_DECRYPT);

	unlink (tmp_out_file);
}

static void open_envelope()
{
	char buffer[100];
	snprintf (buffer, 100, "%s.key", GTK_ENTRY(env_envelope)->text);

	rsa_convert_from_base64 (buffer, "/tmp/des.key.rsa");
	rsa_crypt (GTK_ENTRY(env_prv_key_path)->text, "/tmp/des.key.rsa", "/tmp/des.key", RSA_DECRYPT);
	des_convert_from_base64 (GTK_ENTRY(env_envelope)->text, "/tmp/msg.des");
	des_crypt ("/tmp/msg.des", GTK_ENTRY(env_out_path)->text, des_get_key_from_file ("/tmp/des.key"), DES_DECRYPT);
}

static void generate_envelope()
{
	char buffer[100];

	des_generate_key ("/tmp/des.key");
	des_crypt (GTK_ENTRY(env_in_path)->text, "/tmp/msg.des", des_get_key_from_file ("/tmp/des.key"), DES_ENCRYPT);
	des_convert_to_base64 ("/tmp/msg.des", GTK_ENTRY(env_envelope)->text);
	rsa_crypt (GTK_ENTRY(env_pub_key_path)->text, "/tmp/des.key", "/tmp/des.key.rsa", RSA_ENCRYPT);
	snprintf (buffer, 100, "%s.key", GTK_ENTRY(env_envelope)->text);
	rsa_convert_to_base64 ("/tmp/des.key.rsa", buffer);
}

static void generate_signature()
{
	sha1 (GTK_ENTRY (sgn_in_path)->text, "/tmp/msg.sha");
	rsa_crypt (GTK_ENTRY (sgn_prv_key_path)->text, "/tmp/msg.sha", "/tmp/hash.rsa", RSA_ENCRYPT);
	rsa_convert_to_base64 ("/tmp/hash.rsa", GTK_ENTRY (sgn_signature)->text);
}

static void open_signature()
{
	rsa_convert_from_base64 (GTK_ENTRY (sgn_signature)->text, "/tmp/hash.rsa");
	rsa_crypt (GTK_ENTRY (sgn_pub_key_path)->text, "/tmp/hash.rsa", GTK_ENTRY (sgn_out_path)->text, RSA_DECRYPT);
}

static void generate_stamp()
{
	char buffer[100];

	des_generate_key ("/tmp/des.key");
	des_crypt (GTK_ENTRY(stmp_in_path)->text, "/tmp/msg.des", des_get_key_from_file ("/tmp/des.key"), DES_ENCRYPT);
	des_convert_to_base64 ("/tmp/msg.des", GTK_ENTRY(stmp_env_path)->text);
	rsa_crypt (GTK_ENTRY(stmp_rcv_pub_key_path)->text, "/tmp/des.key", "/tmp/des.key.rsa", RSA_ENCRYPT);
	snprintf (buffer, 100, "%s.key", GTK_ENTRY(stmp_env_path)->text);
	rsa_convert_to_base64 ("/tmp/des.key.rsa", buffer);
	sha1 (GTK_ENTRY (stmp_env_path)->text, "/tmp/msg.sha");
	rsa_crypt (GTK_ENTRY (stmp_snd_prv_key_path)->text, "/tmp/msg.sha", "/tmp/hash.rsa", RSA_ENCRYPT);
	rsa_convert_to_base64 ("/tmp/hash.rsa", GTK_ENTRY (stmp_sgn_path)->text);
}

static void open_stamp()
{
	char buffer[100];
	rsa_convert_from_base64 (GTK_ENTRY (stmp_sgn_path)->text, "/tmp/hash.rsa");
	rsa_crypt (GTK_ENTRY (stmp_snd_pub_key_path)->text, "/tmp/hash.rsa", GTK_ENTRY (stmp_out_path)->text, RSA_DECRYPT);
	
	snprintf (buffer, 100, "%s.key", GTK_ENTRY(stmp_env_path)->text);

	rsa_convert_from_base64 (buffer, "/tmp/des.key.rsa");
	rsa_crypt (GTK_ENTRY(stmp_snd_prv_key_path)->text, "/tmp/des.key.rsa", "/tmp/des.key", RSA_DECRYPT);
	des_convert_from_base64 (GTK_ENTRY(stmp_env_path)->text, "/tmp/msg.des");
	des_crypt ("/tmp/msg.des", GTK_ENTRY(stmp_out_path)->text, des_get_key_from_file ("/tmp/des.key"), DES_DECRYPT);
	
}

static void asym_key_generate_callback()
{
	rsa_generate_key (GTK_ENTRY (asym_pub_key_path)->text, GTK_ENTRY (asym_prv_key_path)->text,
					atoi (GTK_ENTRY (asym_key_length)->text));
}

int main (int argc, char **argv)
{
	/* initialize gtk engine */
	gtk_init (&argc, &argv);
	
	/* main window setup */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 1);
	gtk_widget_set_size_request (window, 640, 450);
	gtk_window_set_title (GTK_WINDOW (window), "GPrivacy");
	g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
	
	/* use vbox to get nice widget layout */
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	
	/* menus setup */
	file_menu = gtk_menu_new();
	quit_item = gtk_menu_item_new_with_label ("Quit");
	gtk_menu_shell_append (GTK_MENU_SHELL (file_menu), quit_item);
	g_signal_connect_swapped (G_OBJECT (quit_item), "activate", G_CALLBACK (gtk_main_quit), NULL);

	edit_menu = gtk_menu_new();
	cut_item = gtk_menu_item_new_with_label ("Cut");
	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), cut_item);
	copy_item = gtk_menu_item_new_with_label ("Copy");
	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), copy_item);
	paste_item = gtk_menu_item_new_with_label ("Paste");
	gtk_menu_shell_append (GTK_MENU_SHELL (edit_menu), paste_item);
	
	help_menu = gtk_menu_new();
	about_item = gtk_menu_item_new_with_label ("About");
	gtk_menu_shell_append (GTK_MENU_SHELL (help_menu), about_item);
	g_signal_connect_swapped (G_OBJECT (about_item), "activate", G_CALLBACK (show_credits), NULL);
	
	/* menu bar setup */
	menu_bar = gtk_menu_bar_new();
	gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 2);
	
	file_item = gtk_menu_item_new_with_label ("File");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_item), file_menu);
	gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), file_item);
	
	edit_item = gtk_menu_item_new_with_label ("Edit");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (edit_item), edit_menu);
	gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), edit_item);
	
	help_item = gtk_menu_item_new_with_label ("Help");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_item), help_menu);
	gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), help_item);
	gtk_menu_item_right_justify (GTK_MENU_ITEM (help_item));

	/* notebook (tabs) setup */
	notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
	gtk_box_pack_end (GTK_BOX (vbox), notebook, TRUE, TRUE, 2);
	
	/* frames setup */
	frame_envelope = gtk_frame_new ("Digital envelope");
	gtk_container_set_border_width (GTK_CONTAINER (frame_envelope), 5);
	label = gtk_label_new ("Digital envelope");
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame_envelope, label);
	
	frame_signature = gtk_frame_new ("Digital signature");
	gtk_container_set_border_width (GTK_CONTAINER (frame_signature), 5);
	label = gtk_label_new ("Digital signature");
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame_signature, label);
	
	frame_stamp = gtk_frame_new ("Digital stamp");
	gtk_container_set_border_width (GTK_CONTAINER (frame_stamp), 5);
	label = gtk_label_new ("Digital stamp");
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame_stamp, label);
	
	frame_symetric = gtk_frame_new ("Symetric cypher (DES)");
	gtk_container_set_border_width (GTK_CONTAINER (frame_symetric), 5);
	label = gtk_label_new ("Symetric cypher");
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame_symetric, label); 
	
	frame_asymetric = gtk_frame_new ("Asymetric cypher (RSA)");
	gtk_container_set_border_width (GTK_CONTAINER (frame_asymetric), 5);
	label = gtk_label_new ("Asymetric cypher");
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame_asymetric, label);
	
	frame_hash = gtk_frame_new ("Hash (SHA-1)");
	gtk_container_set_border_width (GTK_CONTAINER (frame_hash), 5);
	label = gtk_label_new ("Hash");
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame_hash, label);
	
	/* "Symetric" panel setup */
	vbox = gtk_vbox_new (FALSE, 10);
	gtk_container_add (GTK_CONTAINER (frame_symetric), vbox);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Key file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sym_key_path = gtk_entry_new();
	gtk_widget_set_size_request (sym_key_path, 300, 25);
	gtk_entry_set_text (GTK_ENTRY (sym_key_path), "des_kljuc.txt");
	gtk_entry_set_max_length (GTK_ENTRY (sym_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), sym_key_path, FALSE, FALSE, 0);
	
	sym_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sym_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sym_key_select), "clicked", G_CALLBACK (file_select), sym_key_path);
	
	sym_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sym_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sym_key_view), "clicked", G_CALLBACK (view_file), sym_key_path);
	
	sym_key_generate = gtk_button_new_with_label ("Generate");
	gtk_box_pack_start (GTK_BOX (hbox), sym_key_generate, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sym_key_generate), "clicked", G_CALLBACK (sym_key_generate_callback), NULL);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Input file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sym_in_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (sym_in_path), "des_ulaz.txt");
	gtk_widget_set_size_request (sym_in_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (sym_in_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), sym_in_path, FALSE, FALSE, 0);
	
	sym_in_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sym_in_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sym_in_select), "clicked", G_CALLBACK (file_select), sym_in_path);
	
	sym_in_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sym_in_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sym_in_view), "clicked", G_CALLBACK (view_file), sym_in_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Output file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sym_out_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (sym_out_path), "des_izlaz.txt");
	gtk_entry_set_max_length (GTK_ENTRY (sym_out_path), 100);
	gtk_widget_set_size_request (sym_out_path, 300, 25);
	gtk_box_pack_start (GTK_BOX (hbox), sym_out_path, FALSE, FALSE, 0);
	
	sym_out_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sym_out_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sym_out_select), "clicked", G_CALLBACK (file_select), sym_out_path);
	
	sym_out_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sym_out_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sym_out_view), "clicked", G_CALLBACK (view_file), sym_out_path);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);

	sym_encrypt = gtk_button_new_with_label ("Encrypt");
	gtk_widget_set_size_request (sym_encrypt, 110, 30);
	gtk_box_pack_start (GTK_BOX (hbox), sym_encrypt, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (sym_encrypt), "clicked", G_CALLBACK (sym_encrypt_callback), NULL);
	
	sym_decrypt = gtk_button_new_with_label ("Decrypt");
	gtk_widget_set_size_request (sym_decrypt, 110, 30);
	gtk_box_pack_start (GTK_BOX (hbox), sym_decrypt, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (sym_decrypt), "clicked", G_CALLBACK (sym_decrypt_callback), NULL);
	
	/* "Asymetric" panel setup */
	vbox = gtk_vbox_new (FALSE, 10);
	gtk_container_add (GTK_CONTAINER (frame_asymetric), vbox);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Private key");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	asym_prv_key_path = gtk_entry_new();
	gtk_widget_set_size_request (asym_prv_key_path, 300, 25);
	gtk_entry_set_text (GTK_ENTRY (asym_prv_key_path), "rsa_tajni_kljuc.txt");
	gtk_entry_set_max_length (GTK_ENTRY (asym_prv_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), asym_prv_key_path, FALSE, FALSE, 0);
	
	asym_prv_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), asym_prv_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_prv_key_select), "clicked", G_CALLBACK (file_select), asym_prv_key_path);
	
	asym_prv_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), asym_prv_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_prv_key_view), "clicked", G_CALLBACK (view_file), asym_prv_key_path);
	
	asym_prv_key_generate = gtk_button_new_with_label ("Generate");
	gtk_box_pack_start (GTK_BOX (hbox), asym_prv_key_generate, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_prv_key_generate), "clicked", G_CALLBACK (asym_key_generate_callback), NULL);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Public key");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	asym_pub_key_path = gtk_entry_new();
	gtk_widget_set_size_request (asym_pub_key_path, 300, 25);
	gtk_entry_set_text (GTK_ENTRY (asym_pub_key_path), "rsa_javni_kljuc.txt");
	gtk_entry_set_max_length (GTK_ENTRY (asym_pub_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), asym_pub_key_path, FALSE, FALSE, 0);
	
	asym_pub_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), asym_pub_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_pub_key_select), "clicked", G_CALLBACK (file_select), asym_pub_key_path);
	
	asym_pub_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), asym_pub_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_pub_key_view), "clicked", G_CALLBACK (view_file), asym_pub_key_path);
	
	asym_pub_key_generate = gtk_button_new_with_label ("Generate");
	gtk_box_pack_start (GTK_BOX (hbox), asym_pub_key_generate, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_pub_key_generate), "clicked", G_CALLBACK (asym_key_generate_callback), NULL);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Input file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	asym_in_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (asym_in_path), "rsa_ulaz.txt");
	gtk_widget_set_size_request (asym_in_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (asym_in_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), asym_in_path, FALSE, FALSE, 0);
	
	asym_in_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), asym_in_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_in_select), "clicked", G_CALLBACK (file_select), asym_in_path);
	
	asym_in_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), asym_in_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_in_view), "clicked", G_CALLBACK (view_file), asym_in_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Output file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	asym_out_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (asym_out_path), "rsa_izlaz.txt");
	gtk_entry_set_max_length (GTK_ENTRY (asym_out_path), 100);
	gtk_widget_set_size_request (asym_out_path, 300, 25);
	gtk_box_pack_start (GTK_BOX (hbox), asym_out_path, FALSE, FALSE, 0);
	
	asym_out_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), asym_out_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_out_select), "clicked", G_CALLBACK (file_select), asym_out_path);
	
	asym_out_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), asym_out_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (asym_out_view), "clicked", G_CALLBACK (view_file), asym_out_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Key length");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);

	asym_key_length = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (asym_key_length), "128");
	gtk_entry_set_max_length (GTK_ENTRY (asym_key_length), 4);
	gtk_widget_set_size_request (asym_key_length, 50, 25);
	gtk_box_pack_start (GTK_BOX (hbox), asym_key_length, FALSE, FALSE, 0);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);
	
	asym_encrypt = gtk_button_new_with_label ("Encrypt");
	gtk_widget_set_size_request (asym_encrypt, 110, 30);
	gtk_box_pack_start (GTK_BOX (hbox), asym_encrypt, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (asym_encrypt), "clicked", G_CALLBACK (asym_encrypt_callback), NULL);
	
	asym_decrypt = gtk_button_new_with_label ("Decrypt");
	gtk_widget_set_size_request (asym_decrypt, 110, 30);
	gtk_box_pack_start (GTK_BOX (hbox), asym_decrypt, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (asym_decrypt), "clicked", G_CALLBACK (asym_decrypt_callback), NULL);

	/* "Hash" panel setup */
	vbox = gtk_vbox_new (FALSE, 10);
	gtk_container_add (GTK_CONTAINER (frame_hash), vbox);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Input file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	hsh_in_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (hsh_in_path), "sha_ulaz.txt");
	gtk_widget_set_size_request (hsh_in_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (hsh_in_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), hsh_in_path, FALSE, FALSE, 0);
	
	hsh_in_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), hsh_in_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (hsh_in_select), "clicked", G_CALLBACK (file_select), hsh_in_path);
	
	hsh_in_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), hsh_in_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (hsh_in_view), "clicked", G_CALLBACK (view_file), hsh_in_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Output file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	hsh_out_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (hsh_out_path), "sha_izlaz.txt");
	gtk_widget_set_size_request (hsh_out_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (hsh_out_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), hsh_out_path, FALSE, FALSE, 0);
	
	hsh_out_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), hsh_out_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (hsh_out_select), "clicked", G_CALLBACK (file_select), hsh_out_path);
	
	hsh_out_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), hsh_out_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (hsh_out_view), "clicked", G_CALLBACK (view_file), hsh_out_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Hash");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	hsh_hash = gtk_entry_new();
	gtk_widget_set_size_request (hsh_hash, 300, 25);
	gtk_box_pack_start (GTK_BOX (hbox), hsh_hash, FALSE, FALSE, 0);
	
	hsh_calculate = gtk_button_new_with_label ("Calculate hash");
	gtk_box_pack_start (GTK_BOX (hbox), hsh_calculate, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (hsh_calculate), "clicked", G_CALLBACK (hsh_callback), NULL);
	
	/* "Envelope" panel setup */
	vbox = gtk_vbox_new (FALSE, 10);
	gtk_container_add (GTK_CONTAINER (frame_envelope), vbox);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Input file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	env_in_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (env_in_path), "ulaz.txt");
	gtk_widget_set_size_request (env_in_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (env_in_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), env_in_path, FALSE, FALSE, 0);
	
	env_in_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), env_in_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_in_select), "clicked", G_CALLBACK (file_select), env_in_path);
	
	env_in_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), env_in_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_in_view), "clicked", G_CALLBACK (view_file), env_in_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Output file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	env_out_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (env_out_path), "izlaz.txt");
	gtk_widget_set_size_request (env_out_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (env_out_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), env_out_path, FALSE, FALSE, 0);
	
	env_out_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), env_out_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_out_select), "clicked", G_CALLBACK (file_select), env_out_path);
	
	env_out_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), env_out_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_out_view), "clicked", G_CALLBACK (view_file), env_out_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Private key");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	env_prv_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (env_prv_key_path), "rsa_b_tajni.txt");
	gtk_widget_set_size_request (env_prv_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (env_prv_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), env_prv_key_path, FALSE, FALSE, 0);
	
	env_prv_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), env_prv_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_prv_key_select), "clicked", G_CALLBACK (file_select), env_prv_key_path);
	
	env_prv_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), env_prv_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_prv_key_view), "clicked", G_CALLBACK (view_file), env_prv_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Public key");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	env_pub_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (env_pub_key_path), "rsa_b_javni.txt");
	gtk_widget_set_size_request (env_pub_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (env_pub_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), env_pub_key_path, FALSE, FALSE, 0);
	
	env_pub_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), env_pub_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_pub_key_select), "clicked", G_CALLBACK (file_select), env_pub_key_path);
	
	env_pub_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), env_pub_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_pub_key_view), "clicked", G_CALLBACK (view_file), env_pub_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Envelope");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	env_envelope = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (env_envelope), "omotnica.txt");
	gtk_widget_set_size_request (env_envelope, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (env_envelope), 100);
	gtk_box_pack_start (GTK_BOX (hbox), env_envelope, FALSE, FALSE, 0);
	
	env_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), env_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_select), "clicked", G_CALLBACK (file_select), env_envelope);
	
	env_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), env_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (env_view), "clicked", G_CALLBACK (view_file), env_envelope);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	env_open = gtk_button_new_with_label ("Open envelope");
	gtk_box_pack_start (GTK_BOX (hbox), env_open, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (env_open), "clicked", G_CALLBACK (open_envelope), NULL);
	
	env_generate = gtk_button_new_with_label ("Generate envelope");
	gtk_box_pack_start (GTK_BOX (hbox), env_generate, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (env_generate), "clicked", G_CALLBACK (generate_envelope), NULL);
	
	/* "Signature" panel setup */
	vbox = gtk_vbox_new (FALSE, 10);
	gtk_container_add (GTK_CONTAINER (frame_signature), vbox);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Input file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sgn_in_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (sgn_in_path), "ulaz.txt");
	gtk_widget_set_size_request (sgn_in_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (sgn_in_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), sgn_in_path, FALSE, FALSE, 0);
	
	sgn_in_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_in_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_in_select), "clicked", G_CALLBACK (file_select), sgn_in_path);
	
	sgn_in_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_in_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_in_view), "clicked", G_CALLBACK (view_file), sgn_in_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Output file");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sgn_out_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (sgn_out_path), "izlaz.txt");
	gtk_widget_set_size_request (sgn_out_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (sgn_out_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), sgn_out_path, FALSE, FALSE, 0);
	
	sgn_out_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_out_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_out_select), "clicked", G_CALLBACK (file_select), sgn_out_path);

	sgn_out_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_out_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_out_view), "clicked", G_CALLBACK (view_file), sgn_out_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Private key");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sgn_prv_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (sgn_prv_key_path), "rsa_a_tajni.txt");
	gtk_widget_set_size_request (sgn_prv_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (sgn_prv_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), sgn_prv_key_path, FALSE, FALSE, 0);
	
	sgn_prv_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_prv_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_prv_key_select), "clicked", G_CALLBACK (file_select), sgn_prv_key_path);
	
	sgn_prv_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_prv_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_prv_key_view), "clicked", G_CALLBACK (view_file), sgn_prv_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Public key");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sgn_pub_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (sgn_pub_key_path), "rsa_a_javni.txt");
	gtk_widget_set_size_request (sgn_pub_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (sgn_pub_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), sgn_pub_key_path, FALSE, FALSE, 0);
	
	sgn_pub_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_pub_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_pub_key_select), "clicked", G_CALLBACK (file_select), sgn_pub_key_path);
	
	sgn_pub_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_pub_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_pub_key_view), "clicked", G_CALLBACK (view_file), sgn_pub_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Digital sign");
	gtk_widget_set_size_request (label, 100, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	sgn_signature = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (sgn_signature), "potpis.txt");
	gtk_widget_set_size_request (sgn_signature, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (sgn_signature), 100);
	gtk_box_pack_start (GTK_BOX (hbox), sgn_signature, FALSE, FALSE, 0);
	
	sgn_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_select, FALSE, FALSE, 0);
    g_signal_connect (G_OBJECT (sgn_select), "clicked", G_CALLBACK (file_select), sgn_signature);
	
	sgn_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (sgn_view), "clicked", G_CALLBACK (view_file), sgn_signature);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	sgn_generate = gtk_button_new_with_label ("Generate signature");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_generate, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (sgn_generate), "clicked", G_CALLBACK (generate_signature), NULL);
	
	sgn_check = gtk_button_new_with_label ("Check signature");
	gtk_box_pack_start (GTK_BOX (hbox), sgn_check, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (sgn_check), "clicked", G_CALLBACK (open_signature), NULL);
	
	/* "Stamp" panel setup */
	vbox = gtk_vbox_new (FALSE, 10);
	gtk_container_add (GTK_CONTAINER (frame_stamp), vbox);

	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Input file");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_in_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_in_path), "ulaz.txt");
	gtk_widget_set_size_request (stmp_in_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_in_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_in_path, FALSE, FALSE, 0);
	
	stmp_in_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_in_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_in_select), "clicked", G_CALLBACK (file_select), stmp_in_path);

	stmp_in_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_in_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_in_view), "clicked", G_CALLBACK (view_file), stmp_in_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Output file");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_out_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_out_path), "izlaz.txt");
	gtk_widget_set_size_request (stmp_out_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_out_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_out_path, FALSE, FALSE, 0);
	
	stmp_out_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_out_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_out_select), "clicked", G_CALLBACK (file_select), stmp_out_path);
	
	stmp_out_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_out_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_out_view), "clicked", G_CALLBACK (view_file), stmp_out_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Sign file");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_sgn_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_sgn_path), "pecat.txt");
	gtk_widget_set_size_request (stmp_sgn_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_sgn_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_sgn_path, FALSE, FALSE, 0);
	
	stmp_sgn_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_sgn_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_sgn_select), "clicked", G_CALLBACK (file_select), stmp_sgn_path);

	stmp_sgn_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_sgn_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_sgn_view), "clicked", G_CALLBACK (view_file), stmp_sgn_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Envelope file");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_env_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_env_path), "omotnica.txt");
	gtk_widget_set_size_request (stmp_env_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_env_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_env_path, FALSE, FALSE, 0);
	
	stmp_env_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_env_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_env_select), "clicked", G_CALLBACK (file_select), stmp_env_path);
		
	stmp_env_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_env_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_env_view), "clicked", G_CALLBACK (view_file), stmp_env_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Sender public key");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_snd_pub_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_snd_pub_key_path), "rsa_a_javni.txt");
	gtk_widget_set_size_request (stmp_snd_pub_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_snd_pub_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_snd_pub_key_path, FALSE, FALSE, 0);
	
	stmp_snd_pub_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_snd_pub_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_snd_pub_key_select), "clicked", G_CALLBACK (file_select), stmp_snd_pub_key_path);
	
	stmp_snd_pub_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_snd_pub_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_snd_pub_key_view), "clicked", G_CALLBACK (view_file), stmp_snd_pub_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Sender private key");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_snd_prv_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_snd_prv_key_path), "rsa_a_tajni.txt");
	gtk_widget_set_size_request (stmp_snd_prv_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_snd_prv_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_snd_prv_key_path, FALSE, FALSE, 0);
	
	stmp_snd_prv_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_snd_prv_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_snd_prv_key_select), "clicked", G_CALLBACK (file_select), stmp_snd_prv_key_path);
	
	stmp_snd_prv_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_snd_prv_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_snd_prv_key_view), "clicked", G_CALLBACK (view_file), stmp_snd_prv_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Receiver public key");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_rcv_pub_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_rcv_pub_key_path), "rsa_b_javni.txt");
	gtk_widget_set_size_request (stmp_rcv_pub_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_rcv_pub_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_rcv_pub_key_path, FALSE, FALSE, 0);
	
	stmp_rcv_pub_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_rcv_pub_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_rcv_pub_key_select), "clicked", G_CALLBACK (file_select), stmp_rcv_pub_key_path);
	stmp_rcv_pub_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_rcv_pub_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_rcv_pub_key_view), "clicked", G_CALLBACK (view_file), stmp_rcv_pub_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	label = gtk_label_new ("Receiver private key");
	gtk_widget_set_size_request (label, 140, 20);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
	stmp_rcv_prv_key_path = gtk_entry_new();
	gtk_entry_set_text (GTK_ENTRY (stmp_rcv_prv_key_path), "rsa_b_tajni.txt");
	gtk_widget_set_size_request (stmp_rcv_prv_key_path, 300, 25);
	gtk_entry_set_max_length (GTK_ENTRY (stmp_rcv_prv_key_path), 100);
	gtk_box_pack_start (GTK_BOX (hbox), stmp_rcv_prv_key_path, FALSE, FALSE, 0);
	
	stmp_rcv_prv_key_select = gtk_button_new_with_label ("Select");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_rcv_prv_key_select, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_rcv_prv_key_select), "clicked", G_CALLBACK (file_select), stmp_rcv_prv_key_path);
	stmp_rcv_prv_key_view = gtk_button_new_with_label ("View");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_rcv_prv_key_view, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (stmp_rcv_prv_key_view), "clicked", G_CALLBACK (view_file), stmp_rcv_prv_key_path);
	
	hbox = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	
	stmp_generate = gtk_button_new_with_label ("Generate stamp");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_generate, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (stmp_generate), "clicked", G_CALLBACK (generate_stamp), NULL);
	
	stmp_open = gtk_button_new_with_label ("Open stamp");
	gtk_box_pack_start (GTK_BOX (hbox), stmp_open, TRUE, TRUE, 0);
	g_signal_connect (G_OBJECT (stmp_open), "clicked", G_CALLBACK (open_stamp), NULL);
	
	/* Show all widgets and run the engine */
	gtk_widget_show_all (window);
	
	gtk_main();
	
	return 0;
}
