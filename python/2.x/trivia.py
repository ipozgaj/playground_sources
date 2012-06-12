import xchat, re

__module_name__ = 'triviacheater'
__module_version__ = '0.1'
__module_description__ = 'trivia quiz cheater'

bot_name = 'ZecBot'
database_file = 'baza.txt'

def chan_msg_hook(word, word_eol, userdata):
	author, message = word[0], word[1]
	if author != bot_name:
		return xchat.EAT_NONE
	ignore_words = ['hint', 'score', 'Odgovor']
	for w in ignore_words:
		if re.compile(w).findall(message):
			return xchat.EAT_NONE
	nums = re.compile('\d+').findall(message)
	if nums and len(nums) > 1:
		question_num = nums[1]
		if question_num < 10:
			return xchat.EAT_NONE
		answer = ''
		try:
			for s in database[question_num]:
				answer = answer + s + ' '
			answer = answer[:-1]
			xchat.hook_timer(400 * len(answer), timer_cb, answer.lower())
		except KeyError:
			pass
	
def timer_cb(data):
	xchat.command('say ' + data)

print '*' * 31
print 'Trivia Quiz Cheater version ' + __module_version__
print '*' * 31
database = {}
for line in file(database_file):
	data = line.rstrip().split()
	database[data[0]] = data[1:]

xchat.hook_print("Channel Message", chan_msg_hook)
