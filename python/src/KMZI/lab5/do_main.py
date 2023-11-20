import client, register, counter

number_users = 10

r1 = register.Register()
c1 = client.Client()
co1 = counter.Counter()

r1.public_list_r()

for i in range(number_users):
    id_ = r1.get_uniq_id_c(c1.get_uniq_id(i ** 2 + 1))
    co1.get_list_cs(r1.send_uniq_id_cs())
    
    vote1 = c1.gen_name_of_izb()
    c1.gen_send_pair_MB(vote1, id_)
    MB_B = c1.send_end_vote(id_)
    co1.public_M_MB(MB_B)
    
    co1.init_key(c1.send_key_id(id_))    
    co1.find_res(co1.RC2_decrypt(co1.list_M_MB[1][1]))

print(co1.list_res)