import xchat, re

__module_name__ = 'kicker'
__module_version__ = '0.1'
__module_description__ = 'kicks on forbidden words'

forbidden_words = ['martin', 'strpic', 'ogame', 'pixica']

def chan_msg_hook(word, word_eol, userdata):
	author, message = word[0], word[1]
	for w in forbidden_words:
		if w.lower() in message.lower():
			xchat.command('kick ' + author + ' ' + 'forbidden word: ' + w)
	

xchat.hook_print("Channel Message", chan_msg_hook)
