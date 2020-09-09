qreg q[5];
creg c[3];
h q[0];
h q[1];
h q[2];
s q[0];
s q[1];
s q[2];
cx q[1],q[2];
tdg q[2];
cx q[0],q[2];
t q[2];
end