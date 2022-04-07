create database if NOT exists stock_day;

USE stock_day;

CREATE TABLE IF NOT EXISTS index_name_match(
	ATOCK_INDEX INT NOT NULL primary key,
    CHINESE_NAME varchar(200) not null
);



-- insert template 
insert ignore into index_name_match () values ("81165","框架是刚刚");

select * from index_name_match;
delete from index_name_match where ATOCK_INDEX=81165;

CREATE TABLE IF NOT EXISTS stock_day_data(
	ATOCK_INDEX INT NOT NULL,
    DAY_DATE varchar(15) not null,
    DAY_STATRT varchar(15) not null,
    DAY_END varchar(15) not null,
    DAY_MAX varchar(15) not null,
	DAY_MIN varchar(15) not null,
	DAY_TRAYNING_VALUME varchar(20) not null,
    primary key(ATOCK_INDEX,DAY_DATE)
);

insert ignore into stock_day_data () values ();
insert ignore into stock_day_data () values ("2022-03-09", "1764.100", "1779.180", "1805.000", "1721.110", "56434.000");
insert ignore into stock_day_data () values ("600523","2020-03-09", "14.013", "13.113", "14.113", "13.033", "119638.000");
select * from stock_day_data where ATOCK_INDEX=600523;

USE stock_min;

CREATE TABLE IF NOT EXISTS index_name_match(
	ATOCK_INDEX INT NOT NULL primary key,
    CHINESE_NAME varchar(200) not null
);

CREATE TABLE IF NOT EXISTS stock_min_600519(
	DATEplusMIN_MIN varchar(15) NOT NULL,
    STATRT_MIN varchar(15) not null,
    END_MIN varchar(15) not null,
    MAX_MIN varchar(15) not null,
    MIN_MIN varchar(15) not null,
    TRANING_TOTAL_MIN varchar(15) not null,
	EXCHANGE_RATIO varchar(20) not null,
    primary key(DATEplusMIN_MIN)
);

insert ignore into stock_min_600519 () values ("202203091045","1788.00", "1786.61", "1793.50", "1780.00", "1550.95", {}, 1.2346);
insert ignore into stock_min_600519 () values (202204061500, "1757.88", "1765.00", "1765.03", "1757.10", "1903.00", "1.51");
select * from stock_min_600521;

-- insert template 
insert ignore into index_name_match () values ();

