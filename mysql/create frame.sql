create database if NOT exists stock_day;

USE stock_day;

CREATE TABLE IF NOT EXISTS index_name_match(
	ATOCK_INDEX INT NOT NULL primary key,
    CHINESE_NAME varchar(200) not null
);

-- insert template 
insert ignore into index_name_match () values ();

create database if NOT exists stock_min;

USE stock_min;

CREATE TABLE IF NOT EXISTS index_name_match(
	ATOCK_INDEX INT NOT NULL primary key,
    CHINESE_NAME varchar(200) not null
);

-- insert template 
insert ignore into index_name_match () values ();

