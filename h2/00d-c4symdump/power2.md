```
1: #include <stdio.h>
2: int power2(int n) {
3:    int r, i;
4:    r = 1;
    ENT  2
    LLA  -1
    PSH
    IMM  1
    SI
5:    i = 1;
    LLA  -2
    PSH
    IMM  1
    SI
6:    while (i<=n) {
    LLA  -2
    LI
    PSH
    LLA  2
    LI
    LE
    BZ   0
7:       r = r*2;
    LLA  -1
    PSH
    LLA  -1
    LI
    PSH
    IMM  2
    MUL
    SI
8:       i++;
    LLA  -2
    PSH
    LI
    PSH
    IMM  1
    ADD
    SI
    PSH
    IMM  1
    SUB
9:    }
10:    return r;
    JMP  -447217528
    LLA  -1
    LI
    LEV
11: }
    LEV
12:
13: int main() {
14:    printf("power2(3)=%d\n", power2(3));
    ENT  0
    IMM  -447483888
    PSH
    IMM  3
    PSH
    JSR  -447217640
    ADJ  1
    PSH
    PRTF
    ADJ  2
---
ENT  0
2> IMM  -16863216
3> PSH
4> IMM  3
5> PSH
6> JSR  -16596968
7> ENT  2
8> LLA  -1
9> PSH
10> IMM  1
11> SI
12> LLA  -2
13> PSH
14> IMM  1
15> SI
16> LLA  -2
17> LI
18> PSH
19> LLA  2
20> LI
21> LE
22> BZ   -16596568
23> LLA  -1
24> PSH
25> LLA  -1
26> LI
27> PSH
28> IMM  2
29> MUL
30> SI
31> LLA  -2
32> PSH
33> LI
34> PSH
35> IMM  1
36> ADD
37> SI
38> PSH
39> IMM  1
40> SUB
41> JMP  -16596856
42> LLA  -2
43> LI
44> PSH
45> LLA  2
46> LI
47> LE
48> BZ   -16596568
49> LLA  -1
50> PSH
51> LLA  -1
52> LI
53> PSH
54> IMM  2
55> MUL
56> SI
57> LLA  -2
58> PSH
59> LI
60> PSH
61> IMM  1
62> ADD
63> SI
64> PSH
65> IMM  1
66> SUB
67> JMP  -16596856
68> LLA  -2
69> LI
70> PSH
71> LLA  2
72> LI
73> LE
74> BZ   -16596568
75> LLA  -1
76> PSH
77> LLA  -1
78> LI
79> PSH
80> IMM  2
81> MUL
82> SI
83> LLA  -2
84> PSH
85> LI
86> PSH
87> IMM  1
88> ADD
89> SI
90> PSH
91> IMM  1
92> SUB
93> JMP  -16596856
94> LLA  -2
95> LI
96> PSH
97> LLA  2
98> LI
99> LE
100> BZ   -16596568
101> LLA  -1
102> LI
103> LEV
104> ADJ  1
105> PSH
106> PRTF
power2(3)=8
107> ADJ  2
108> LEV
109> PSH
110> EXIT
exit(12) cycle = 110
```
