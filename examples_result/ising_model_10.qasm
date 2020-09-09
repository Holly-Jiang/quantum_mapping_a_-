OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
creg c[16];
h q[1];
h q[2];
rz(0.864) q[1];
rz(-1.056) q[3];
rz(-0.576) q[5];
rz(-0.288) q[7];
rz(1.248) q[8];
rz(0.9) q[1];
rz(1.08) q[3];
h q[2];
rz(-0.78) q[2];
rz(-0.36) q[2];
h q[3];
h q[4];
rz(0.36) q[3];
rz(-0.12) q[4];
rz(-1.44) q[3];
cx q[2],q[3];
rz(0.72) q[3];
cx q[4],q[5];
cx q[2],q[3];
rz(-0.26) q[2];
rz(1.04) q[2];
rz(-0.26) q[3];
h q[5];
h q[5];
rz(0.12) q[5];
h q[5];
rz(-0.48) q[5];
rz(1.152) q[2];
rz(-1.92) q[6];
cx q[4],q[5];
h q[0];
rz(-1.08) q[2];
h q[6];
rz(0.24) q[5];
rz(-0.3) q[0];
rz(0.78) q[4];
cx q[0],q[1];
rz(0.26) q[4];
rz(-1.04) q[4];
h q[2];
rz(0.38) q[5];
h q[1];
cx q[3],q[4];
rz(0.3) q[1];
rz(-1.2) q[1];
h q[4];
h q[1];
h q[3];
rz(0.6) q[1];
cx q[0],q[1];
h q[3];
rz(0.26) q[1];
rz(-0.78) q[7];
rz(-1.92) q[4];
cx q[1],q[2];
h q[4];
rz(-0.52) q[2];
rz(-1.44) q[4];
rz(0.36) q[5];
h q[0];
cx q[7],q[8];
rz(-1.44) q[5];
rz(-1.92) q[0];
rz(-4.32) q[3];
h q[0];
rz(-0.36) q[4];
cx q[6],q[7];
cx q[1],q[2];
rz(-1.32) q[7];
rz(-0.288) q[5];
rz(0.768) q[6];
rz(-0.144) q[7];
rz(0.624) q[8];
rz(1.8) q[1];
h q[1];
rz(-1.92) q[1];
rz(0.72) q[5];
cx q[6],q[7];
rz(-1.92) q[2];
rz(0.576) q[2];
rz(-1.14) q[6];
cx q[0],q[1];
h q[6];
h q[2];
rz(0.52) q[4];
rz(0.22) q[6];
h q[7];
rz(-0.288) q[0];
cx q[3],q[4];
rz(-0.22) q[7];
rz(0.78) q[1];
rz(-3.6) q[1];
rz(-0.9) q[0];
h q[3];
rz(0.88) q[7];
rz(-1.92) q[3];
cx q[6],q[7];
cx q[0],q[1];
rz(-0.44) q[7];
rz(0.432) q[1];
cx q[6],q[7];
rz(1.5) q[1];
h q[4];
rz(-0.38) q[6];
rz(1.52) q[6];
rz(-0.26) q[7];
cx q[4],q[5];
rz(-0.96) q[6];
rz(3.12) q[2];
rz(-0.72) q[4];
cx q[4],q[5];
h q[6];
rz(-0.528) q[3];
rz(-0.78) q[3];
rz(1.8) q[3];
rz(-3.12) q[4];
rz(0.6) q[5];
rz(1.14) q[5];
h q[7];
rz(-1.92) q[8];
rz(1.536) q[6];
cx q[5],q[6];
h q[0];
rz(-2.28) q[6];
rz(-0.96) q[0];
rz(-0.66) q[7];
h q[0];
cx q[2],q[3];
h q[6];
rz(-0.6) q[4];
cx q[8],q[9];
rz(-1.5) q[0];
rz(-1.8) q[2];
rz(-0.96) q[7];
cx q[1],q[2];
rz(-0.76) q[6];
rz(-1.56) q[2];
rz(-0.96) q[4];
cx q[1],q[2];
rz(1.3) q[4];
cx q[3],q[4];
cx q[5],q[6];
rz(-0.144) q[0];
rz(2.16) q[3];
cx q[5],q[6];
cx q[3],q[4];
rz(-1.92) q[5];
rz(-1.3) q[2];
cx q[2],q[3];
h q[3];
rz(0.66) q[6];
rz(-1.3) q[3];
rz(-0.96) q[1];
rz(4.4) q[7];
h q[1];
rz(-0.96) q[2];
rz(1.56) q[4];
rz(4.56) q[6];
h q[4];
rz(-1.9) q[6];
h q[2];
h q[5];
cx q[1],q[2];
rz(-0.96) q[3];
rz(-0.96) q[5];
rz(-6) q[1];
h q[5];
cx q[1],q[2];
cx q[0],q[1];
rz(3) q[1];
h q[6];
cx q[3],q[4];
cx q[0],q[1];
rz(-7.2) q[3];
cx q[3],q[4];
rz(3.6) q[3];
cx q[2],q[3];
rz(-2.4) q[5];
h q[2];
cx q[5],q[6];
h q[8];
h q[9];
rz(-5.2) q[4];
cx q[2],q[3];
rz(1.1) q[6];
rz(0.08) q[8];
rz(-0.08) q[9];
h q[3];
rz(5.2) q[2];
cx q[6],q[7];
rz(0.32) q[9];
h q[7];
rz(1.2) q[5];
rz(-2.2) q[7];
cx q[8],q[9];
rz(-2.6) q[2];
cx q[6],q[7];
rz(-0.16) q[9];
rz(-0) q[3];
rz(-0) q[5];
rz(0) q[6];
h q[3];
rz(-0) q[0];
h q[7];
rz(1.3) q[1];
rz(-1.1) q[7];
rz(0.26) q[8];
cx q[8],q[9];
h q[0];
h q[0];
rz(0) q[8];
h q[1];
rz(-1.04) q[8];
rz(2.6) q[4];
h q[8];
h q[4];
h q[9];
h q[2];
cx q[5],q[6];
rz(1.9) q[5];
rz(-1.92) q[9];
h q[5];
rz(7.6) q[6];
h q[9];
rz(-1.824) q[9];
rz(-0.24) q[9];
h q[4];
rz(2.52) q[3];
rz(0) q[2];
rz(0.96) q[9];
cx q[7],q[8];
rz(0.52) q[8];
rz(-0) q[1];
cx q[4],q[5];
cx q[7],q[8];
rz(0.84) q[5];
h q[1];
rz(-3.8) q[6];
h q[7];
rz(-0) q[2];
cx q[4],q[5];
rz(-1.92) q[7];
rz(-8.4) q[1];
rz(-0.4) q[9];
rz(-0) q[3];
h q[8];
rz(0) q[1];
rz(0.24) q[8];
rz(2.64) q[7];
rz(2.1) q[1];
rz(-0) q[4];
rz(-0) q[7];
rz(-10.08) q[3];
rz(-0.48) q[9];
rz(-0) q[0];
cx q[8],q[9];
cx q[2],q[3];
rz(-2.1) q[0];
rz(0.78) q[8];
rz(-2.52) q[2];
rz(-3.12) q[8];
cx q[0],q[1];
cx q[7],q[8];
rz(4.2) q[1];
cx q[5],q[6];
rz(1.56) q[8];
cx q[0],q[1];
h q[6];
h q[9];
h q[8];
rz(-0) q[4];
rz(-3.08) q[7];
rz(-0) q[6];
rz(-0.96) q[9];
rz(5.04) q[3];
h q[6];
h q[9];
rz(1.68) q[5];
rz(7.28) q[2];
rz(-0) q[5];
rz(-0.912) q[9];
cx q[2],q[3];
h q[5];
h q[7];
h q[7];
rz(-0.96) q[8];
rz(-3.64) q[2];
rz(1.82) q[1];
rz(-0.84) q[4];
h q[8];
rz(-1.82) q[3];
rz(0.4) q[8];
rz(-1.82) q[2];
rz(-0) q[7];
rz(1.6) q[9];
rz(-2.66) q[6];
cx q[8],q[9];
rz(10.64) q[6];
rz(-0.8) q[9];
cx q[1],q[2];
cx q[8],q[9];
cx q[1],q[2];
cx q[4],q[5];
rz(1.3) q[8];
rz(1.54) q[6];
rz(-1.3) q[7];
cx q[3],q[4];
rz(-5.2) q[8];
cx q[7],q[8];
h q[0];
cx q[7],q[8];
rz(-3.36) q[5];
rz(2.6) q[8];
h q[0];
rz(1.82) q[4];
cx q[7],q[8];
rz(0.96) q[0];
cx q[7],q[8];
rz(-7.28) q[4];
h q[8];
h q[1];
cx q[4],q[5];
h q[9];
h q[2];
rz(-1.54) q[7];
h q[2];
rz(-0) q[8];
h q[1];
rz(2.66) q[5];
h q[8];
h q[4];
rz(0.96) q[1];
cx q[3],q[4];
rz(-0) q[9];
h q[3];
rz(3.64) q[4];
h q[9];
h q[5];
rz(0.96) q[2];
rz(-0) q[9];
rz(0.96) q[3];
rz(-0.56) q[9];
rz(6.16) q[7];
cx q[5],q[6];
rz(0.56) q[8];
rz(0.96) q[5];
rz(-5.32) q[6];
rz(2.24) q[9];
rz(0.72) q[4];
rz(-0.768) q[6];
rz(0.144) q[7];
rz(-0.624) q[8];
h q[8];
rz(0.96) q[6];
rz(0.96) q[9];
rz(0.96) q[8];
h q[3];
cx q[8],q[9];
rz(-0.576) q[2];
h q[4];
rz(-1.12) q[9];
rz(0.144) q[0];
cx q[8],q[9];
rz(0.96) q[4];
cx q[6],q[7];
cx q[6],q[7];
rz(5.4) q[1];
rz(2.7) q[1];
rz(-0.432) q[1];
rz(1.82) q[8];
rz(-2.7) q[0];
rz(0.528) q[3];
rz(-7.28) q[8];
rz(-10.8) q[1];
rz(3.64) q[8];
cx q[0],q[1];
rz(-1.82) q[7];
rz(6.48) q[3];
rz(-3.24) q[2];
cx q[5],q[6];
cx q[0],q[1];
h q[8];
rz(3.24) q[3];
h q[9];
rz(0.288) q[5];
rz(-1.08) q[4];
rz(-12.96) q[3];
h q[9];
cx q[2],q[3];
h q[5];
h q[6];
cx q[2],q[3];
h q[7];
rz(1.98) q[6];
rz(-4.32) q[5];
h q[6];
cx q[4],q[5];
rz(0.912) q[9];
rz(0.72) q[8];
rz(0.96) q[7];
rz(2.88) q[9];
h q[7];
rz(-2.34) q[2];
cx q[8],q[9];
rz(1.08) q[5];
rz(2.16) q[5];
rz(2.34) q[1];
rz(-1.98) q[7];
rz(-0.72) q[9];
cx q[1],q[2];
rz(9.36) q[2];
cx q[4],q[5];
rz(-2.34) q[3];
rz(7.92) q[7];
rz(-4.68) q[2];
cx q[6],q[7];
cx q[1],q[2];
rz(-3.96) q[7];
cx q[3],q[4];
rz(-9.36) q[4];
rz(-1.44) q[9];
cx q[8],q[9];
cx q[6],q[7];
rz(2.34) q[4];
cx q[5],q[6];
rz(13.68) q[6];
rz(-3.42) q[6];
rz(-2.34) q[7];
rz(3.42) q[5];
rz(4.68) q[4];
cx q[3],q[4];
h q[0];
rz(2.34) q[8];
rz(-6.84) q[6];
rz(1.92) q[0];
rz(-9.36) q[8];
h q[0];
h q[1];
cx q[5],q[6];
h q[2];
rz(1.92) q[1];
cx q[7],q[8];
h q[1];
rz(4.68) q[8];
rz(1.92) q[2];
cx q[7],q[8];
h q[2];
h q[3];
h q[4];
rz(1.92) q[3];
h q[3];
h q[5];
rz(1.92) q[4];
h q[4];
h q[6];
rz(1.92) q[5];
h q[5];
h q[7];
rz(1.92) q[6];
h q[6];
h q[8];
rz(1.92) q[7];
h q[7];
h q[9];
rz(1.92) q[8];
rz(1.92) q[9];
h q[8];
h q[9];
rz(0.288) q[0];
rz(-0.864) q[1];
rz(-1.152) q[2];
rz(1.056) q[3];
rz(1.44) q[4];
rz(0.576) q[5];
rz(-1.536) q[6];
rz(0.288) q[7];
rz(-1.248) q[8];
rz(1.824) q[9];
