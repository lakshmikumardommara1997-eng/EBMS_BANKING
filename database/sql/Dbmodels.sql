create table Customer (
    customer_id number primary key,
    first_name varchar2(50) not null,
    last_name varchar2(50) not null,
    email varchar2(100) unique not null,
    phone_number varchar2(15),
    address varchar2(255),
    city varchar2(50),
    state varchar2(50),
    zip_code varchar2(10),
    created_at timestamp default current_timestamp
);