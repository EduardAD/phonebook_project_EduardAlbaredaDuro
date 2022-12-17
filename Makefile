all: phonebook phonebook_search phonebook_delete phonebook_printout phonebook_add phonebook_close

phonebook: phonebook.c
	gcc phonebook.c -g -o phonebook -lpthread -lrt

phonebook_add: phonebook_add.c
	gcc phonebook_add.c -g -o phonebook_add

phonebook_printout: phonebook_printout.c
	gcc phonebook_printout.c -g -o phonebook_printout

phonebook_delete: phonebook_delete.c
	gcc phonebook_delete.c -g -o phonebook_delete

phonebook_search: phonebook_search.c
	gcc phonebook_search.c -g -o phonebook_search -lrt

phonebook_close: phonebook_close.c
	gcc phonebook_close.c -g -o phonebook_close

clean:
	rm -f phonebook phonebook_search phonebook_delete phonebook_printout phonebook_add phonebook_close communication_channel
