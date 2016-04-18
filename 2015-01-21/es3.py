#!/usr/bin/env python3

'''Scrivere un programma python o uno script bash che scandisca il sottoalbero relativo alle directory passate come
parametri (o alla direcotry corrente se non ci sono parametri) e fornisca in output l'elenco dei file che hanno la
stessa somma MD5 (i.e. l'output del comando md5sum).
In output ogni riga deve mostrare un elenco di pathname realtivi a file che hanno la stessa somma MD5 (che quindi
sono molto molto probabilmente uguali).'''


from collections import defaultdict
import sys
import os
import hashlib


def md5checksum2(file_name):
	with open(file_name) as file_to_check:
		# read contents of the file
		data = file_to_check.read()    
		# pipe contents of the file through
		md5_returned = hashlib.md5(data).hexdigest()
	return md5_returned


def md5checksum(filepath):
	with open(filepath, "rb") as afile:
		m = hashlib.md5()
		data = afile.read()
		m.update(data)
	return m.hexdigest()


def list_files_indent(startpath):
    for root, dirs, files in os.walk(startpath):							#la os.walk(path) salva in root il percorso che gli mandi, in files i file che trova nella cartella
        level = root.replace(startpath, '').count(os.sep)					#e penso che usi dirs per andare poi in ricorsione (penso chiami tipo os.walk(dirs) finchè trova
        indent = ' ' * 4 * (level)											#sottodirectory)
        print('{}{}/'.format(indent, os.path.basename(root)))
        subindent = ' ' * 4 * (level + 1)
        for f in files:
            print('{}{}'.format(subindent, f))
            


def calculate_checksums(search_dir):
	checksums = defaultdict(list)
	for root, dirs, files in os.walk(search_dir):					
		for filename in files:
			path = os.path.join(root, filename)
			checksum = md5checksum(path)
			checksums[checksum].append(path)
	return checksums
		
		
def display_equal_files(checksums):
	for checksum, paths in checksums.items():
		if len(paths) > 1:
			print("MD5 sum: {}".format(checksum))
			for path in paths:
				print("    {}".format(path))





if __name__ == "__main__":
            
	if (len(sys.argv)>1):
		search_dir = sys.argv[1]
	else:
		search_dir = '.'
	
	checksums = calculate_checksums(search_dir)
	display_equal_files(checksums)
		
		
		
		
		
		
		
		
