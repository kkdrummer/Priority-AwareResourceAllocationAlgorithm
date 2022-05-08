1: **Input**: *VMs*: List of VM allocation requests
2: **Input**: *Hosts*: List of Hosts Available
3: **Output** : VM to Host Placement Mapping
4: **sort** *VMs* in the order in which the requests are to be processed
5: **arrange** *Hosts* in three partitions level wise
6: **for each** *r* in *VMs*:
7: $~~~~$*aHLevel* $\leftarrow$ -1
8:   $~~~~$*aHIndex* $\leftarrow$ -1
9:   $~~~~$*pl* $\leftarrow$ *r.priorityLevel*
10:  $~~$*rd* $\leftarrow$ *r.resourceDemand*
11:  $~~$**while** *pl* $\gt 0$:
12:        $~~~~~~~~$*id* $\leftarrow$ host of minimum capacity in *pl* level hosts with capacity $\ge$ *rd*
13:        $~~~~~~~~$**if** *id* $\neq$ -1 :
14:        $~~~~~~~~~~~~$*aHLevel* $\leftarrow$ *pl*
15:        $~~~~~~~~~~~~$*aHIndex* $\leftarrow$ *id*
16:        $~~~~~~~~~~~~$**break**
17:        $~~~~~~~~$**end if**
18:        $~~~~~~~~$*pl*$--$
19:    $~~$**end while**
20:    $~~$**if** *aHLevel* $\neq$ -1
21:        $~~~~~~$*h* $\leftarrow$ host at *aHIndex* in *aHLevel* hosts
22:        $~~~~~~$*h.capacity*$-=$*rd*
23:        $~~~~~~$**sort** hosts of *aHlevel* in increasing order
24:        $~~~~~~$map *r* to *h*
25:    $~~$**end if**   
26: **end for**

