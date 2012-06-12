# displays current playing song in xmms (xchat script)
# (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>

import xchat
from xmms import get_playlist_title, get_playlist_pos
 
__module_name__ = "nowPlaying"
__module_version__ = "1.0"
__module_description__ = "Displays what song is currently playing in Xmms"

song = get_playlist_title (get_playlist_pos())

try:
	xchat.command ("me plays: " + song)
except TypeError:
	xchat.prnt ("Xmms is not running")
