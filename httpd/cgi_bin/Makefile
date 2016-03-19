FIND_PATH=-I/home/common/YP/httpd/sql_connect -I/home/common/YP/httpd/sql_connect/mysql_lib/include -L/home/common/YP/httpd/sql_connect/mysql_lib/lib -lmysqlclient

.PHONY:all
all:math_cgi show_stu_info register_stu_info

math_cgi:math_cgi.c
	gcc -o $@ $^ 

show_stu_info:show_stu_info.cpp /home/common/YP/httpd/sql_connect/sql_connect.cpp
	g++ -o $@ $^ $(FIND_PATH)

register_stu_info:register_stu_info.cpp /home/common/YP/httpd/sql_connect/sql_connect.cpp
	g++ -o $@ $^ $(FIND_PATH)

.PHONY:output
output:
	@cp math_cgi ../output/htdocs/cgi_bin
	@cp show_stu_info ../output/htdocs/cgi_bin
	@cp register_stu_info ../output/htdocs/cgi_bin

.PHONY:clean
clean:
	@rm -rf math_cgi show_stu_info register_stu_info

