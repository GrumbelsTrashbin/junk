#!/usr/bin/env python

def vol(command):
	import mixer
	getattr(mixer, 'vol_%s'%command)()

def intel_vol(command):
	import mixer
	mixer.intel_vol(command)

def music(command):
	import music
	music.command(command)

def main():
	import sys
	command = sys.argv[1:]

	if command[0] == 'vol':
		return intel_vol(command[1])
	if command[0] == 'music':
		return music(command[1])

if __name__ == '__main__':
	main()
