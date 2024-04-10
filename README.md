
The NTG1 was the Mercedes Navigation in the E-Class W211 and others. It was constructed by Harman Becker
in Hamburg. 

The publishing of Data has ceeded in 2018 or 2019 whith approx 1-2 year old data.

This is just some information about the DVD content and files on there. Not that i think
its possible to generate new files from OpenStreetmap based content.

Its just a start of reverse engineering knowledge.


FLDB
====

It seems all files on the DVD are **FLDB** files which is practially something like a tar, non compressing
archive of files.

To extract use

	./fldb -x --outdir /tmp -i filename_on_dvd
	
