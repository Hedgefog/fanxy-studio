/*------------------------------------------------
-------Example for Fanxy Studio-------
------------------------------------------------*/

#include <amxmodx>
#include <simple_const>

public plugin_init()
{
	register_plugin(PLUGIN, VERSION, AUTHOR);
	register_clcmd("say /test", "clcmd_test");
}

public clcmd_test(id)
{
	client_print(id, print_chat, MSG_HELLO);
	
	if(is_user_alive(id))
		client_print(id, print_chat, "You're alive!");
	else
		client_print(id, print_chat, "You're dead!");
}