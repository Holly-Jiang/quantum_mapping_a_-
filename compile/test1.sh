g++ -std=c++11 E:/github/quantum_mapping_a_-/src/main_Zulehner.cpp  -o main_Zulehner
g++ -std=c++11 E:/github/quantum_mapping_a_-/src/main_FY_VF2_Zulehner.cpp  -o main_FY_VF2_Zulehner
g++ -std=c++11 E:/github/quantum_mapping_a_-/src/main_GQL.cpp  -o main_GQL
g++ -std=c++11 E:/github/quantum_mapping_a_-/src/main_OPT_VF2.cpp  -o main_OPT_VF2

./main_Zulehner E:/github/quantum_mapping_a_-/examples_result/C17_204.qasm C17_204
E:/github/SubgraphComparing/build/matching/SubgraphMatching.out -d E:/github/SubgraphComparing/test/sample_dataset/test_case_1.graph -q E:/github/VF2_mapping_opt/data/graphDB/Ex2.my -filter GQL -order GQL -engine LFTJ -num MAX
java -jar E:/github/VF2_mapping_opt/out/artifacts/VF2_deal_data_linux_jar/VF2.jar
java -jar E:/github/VF2_mapping_opt/out/artifacts/VF2_linux_jar/VF2.jar
./main_OPT_VF2 E:/github/quantum_mapping_a_-/examples_result/C17_204.qasm C17_204 
./main_FY_VF2_Zulehner E:/github/quantum_mapping_a_-/examples_result/C17_204.qasm C17_204 
./main_GQL E:/github/quantum_mapping_a_-/examples_result/C17_204.qasm C17_204 


./main_Zulehner /root/graph/new/quantum_mapping_a_-/examples_result/4gt11_84.qasm 4gt11_84
/root/graph/new/SubgraphComparing/build/matching/SubgraphMatching.out -d /root/graph/new/SubgraphComparing/test/sample_dataset/test_case_1.graph -q /root/graph/new/VF2_mapping_opt/data/graphDB/Ex2.my -filter GQL -order GQL -engine LFTJ -num MAX
java -jar /root/graph/new/VF2_mapping_opt/out/artifacts/VF2_deal_data_linux_jar/VF2.jar
java -jar /root/graph/new/VF2_mapping_opt/out/artifacts/VF2_linux_jar/VF2.jar
./main_OPT_VF2 /root/graph/new/quantum_mapping_a_-/examples_result/4gt11_84.qasm 4gt11_84 
./main_FY_VF2_Zulehner /root/graph/new/quantum_mapping_a_-/examples_result/4gt11_84.qasm 4gt11_84 
./main_GQL /root/graph/new/quantum_mapping_a_-/examples_result/4gt11_84.qasm 4gt11_84 