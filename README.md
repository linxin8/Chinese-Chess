# 中国象棋

基于极大极小算法的中国象棋程序

## Demo

![GitHub](https://github.com/linxin8/Chinese-Chess/master/demo.gif "demo")

## 基础博弈算法

### 搜索树

把每个游戏状态抽象成节点，把游戏操作抽象成一条边，双方轮次行动，那么整个游戏过程就可以抽象成一棵树。如下图是井字棋的搜索树，树的根节点是游戏的初始状态，其子节点对应玩家一次行动后的状态，叶子节点表示游戏结束的状态。

``` python
# 获取state的所有子节点
def sub_nodes(state):
    nodes = []
    for action in action_list: # 获取所有动作
        new_node = do_action(state, action)	  # 计算新节点 
        nodes.append(new_node)
     return nodes
        
# 搜索算法
def search(state):
    for node in sub_nodes(state): #遍历子节点
        search(node)
```

![GitHub](https://github.com/linxin8/Chinese-Chess/master/Doc/game-tree.png "game-tree")

### 极小极大搜索

对于每个节点，我们设计一个评价函数，该函数只需要静态地评估局面，不考虑棋面的后续操作，其评价分数越高则代表局势对玩家越有利。那么，在对局过程中，假设玩家与AI都是绝对理性的，玩家永远会选择最终评价最高的动作（极大方），而AI永远会选择最终评价最低的动作（极小方）。由此，局面确定后，通过遍历子树，玩家和AI的行动也是确定的，就可以动态地获取一个局面的评估值，它考虑到到了后续玩家与AI的操作。

```python
# 评估状态是否对玩家有利
def estimate(state):
    # to do

# 返回极大或极小的评估值
def minmax(state, is_maximizing_player):
    if state is terminal_node: # 叶子节点直接返回静态评估结果
        return estimate(state) 
    if is_maximizing_player: # 玩家行动
        value = -∞
        for node in sub_nodes(state): #遍历子节点
            value = max(value, minmax(node, False)) # 永远选择评分最大的子节点，下一步轮到AI行动
        return value
   	else: # AI行动
        value = +∞
        for node in sub_nodes(state): #遍历子节点
            value = min(value, minmax(node, False)) # 永远选择评分最小的子节点，下一步轮到玩家行动
    return value 
```

![GitHub](https://github.com/linxin8/Chinese-Chess/master/Doc/min-max.png "min-max")

### Alpha-Beta 剪枝

Alpha-Beta在极小极大算法的基础上进行剪枝，当遍历到足够好或者足够坏的节点时剪枝。问题的关键在于如果判断一个节点足够好或者足够坏。在遍历时，算法维护两个值: alpha，表示最小值，beta，表示最大值。当前的遍历节点的评估值应该在(alpha, beta)区间内，如果小于等于 alpha，则代表当前节点足够坏，进行 alpha 剪枝；如果大于等于 beta，则表示节点足够好，进行 beta 剪枝。

``` python
def alpha_beta_pruning(state, alpha, beta, is_maximizing_player):
    if state is terminal_node: # 叶子节点直接返回静态评估结果
        return estimate(state) 
    if is_maximizing_player: # 玩家行动
        value = -∞
        for node in sub_nodes(state): #遍历子节点
            value = max(value, minmax(node, alpha, beta, False)) # 永远选择评分最大的子节点，下一步轮到AI行动
            alpha = max(alpha, value)
            if alpha >= beta:
                break;  # beta 剪枝
        return value
   	else: # AI行动
        value = +∞
        for node in sub_nodes(state): #遍历子节点
            value = min(value, minmax(node, alpha, beta, False)) # 永远选择评分最小的子节点，下一步轮到玩家行动      
            beta = min(beta, value)
            if beta <= alpha:
                break;  # alpha 剪枝
    return value 
    
```

![GitHub](https://github.com/linxin8/Chinese-Chess/master/Doc/alph-beta.png "alph-beta")

### NegaMax

NegaMax 是极小极大算法的变体，与极小极大算法下等价，它将极大极小的代码逻辑合并在起来，十分利于编写代码。

``` python
def negamax_alpha_beta(state, alpha, beta):
    if state is terminal_node: # 叶子节点直接返回静态评估结果
        return estimate(state)  
    value = +∞
    for node in sub_nodes(state): #遍历子节点
        value = max(value, -negamax_alpha_beta(node, -beta, -alpha)) # 注意-negamax_alpha_beta(node, -beta, -alpha)的写法
        alpha = max(alpha, value)
        if alpha >= beta:
            break;  # 剪枝
    return value
```

## 基础搜索算法

### 深度优先搜索

深度优先搜索从一个树节点出发，按一定顺序进入子节点递归搜索，直至找到叶子节点，然后回退到父节点。其优点是实现简单，内存占用小，缺点是需要搜索完整棵子树。

```python
def deep_search(state):
    for sub_state in sub_states:
        deep_search(state)
```

### 深度限制搜索

该算法在深度优先搜索算法基础上进行改进，使得搜索到一定深度的子树后，不再进行搜索，直接返回结果。其优点是搜索速度快，不用遍历整棵子树，缺点是由于只搜索了一部分，其搜索结果并不准确。

```python
def deep_search_limit_depth(state, depth):
    if depth <=0 :
        return
    for sub_state in sub_states:
        deep_search(state, depth - 1)
```

### 迭代加深搜索

该算法在深度限制搜索的基础上进行改进，先尝试深度小的搜索，如果速度足够快，则进行尝试深度大的搜索。其优点在于在一定时间限制下，总是能尽可能地进行大深度的深度优先搜索，缺点在于搜索结果不准确。

由于搜索的节点数（时间）与搜索深度成指数关系，所以对于最终搜索深度为N的迭代加深搜索而言，第1、2、...、N-1次的搜索尝试所消耗时间占比小，时间成本可以忽略。通常而言，游戏需要AI在一定时间内作出决策，十分适合采用迭代加深搜索策略。

``` python
def iterative_serach(state):
    depth = 0
    while True:
        depth += 1 	# 增加深度
    	start_time = get_time()
        deep_search_limit_depth(state, depth) # 深度限制搜索
        end_time = get_time() 
        if time_over(start_time, end_time):	# 超时则不再继续搜索
            break;
```

## 子节点遍历顺序优化算法

在 alpha-beta 剪枝中，先遍历的子节点越好，那么剪枝效果越高。所以如果对子节点的遍历顺序进行排序是有必要的。而遍历子节点是十分频繁的步骤，排序算法需要有非常高的效率，才不至于影响搜索速度。主要的排序算法有以下几种：

### 静态评估启发

将所有子节点按照静态评估值进行排序

```python
def compare(state):
    return estimated(state) 

# 获取state的所有子节点
def sub_nodes(state):
    nodes = []
    for action in action_list: # 获取所有动作
        new_node = do_action(state, action)	  # 计算新节点 
        nodes.append(new_node)
    return sort(nodes, key=compare, reverse=True) # 排序，按评分高到低的顺序返回
```

### 历史动作启发

该算法认为历史上的最佳动作很有可能是当前状态下的最佳动作。一个动作可以看出（from_x, from_y, to_x, to_y）四元组，分别表示移动棋子的起始坐标和目标坐标。该算法首先在每个状态搜索出最佳动作后，增加这些动作的权值，在后续的排序中，优先考虑这些动作。

```python
# 根据历史排序
def compare(action):
    return history_table[action]

# 获取state的所有子节点
def sub_nodes(state):
    nodes = []
    action_list = sort(action_list, key=compare, reverse=True) # 对动作进行排序
    for action in action_list: # 获取所有动作
        new_node = do_action(state, action)	  # 计算新节点 
        nodes.append(new_node)
    return nodes

def negamax_alpha_beta(state, alpha, beta，depth):
    if depth == 0 or state is terminal_node: # 剩余深度为0或叶子节点直接返回静态评估结果
        return estimate(state)  
    value = +∞
    best_action = Null
    for node in sub_nodes(state): #遍历子节点
       	result = -negamax_alpha_beta(node, -beta, -alpha);
        if result > value:
            best_action = node.actoin;  # 记录当前的action
            value = result
            alpha = result 
        if alpha >= beta:
            break;  # 剪枝
    history_table[best_action] += depth * depth # 更新权值, 增加的权值与depth有关，depth越大，权值越大
    return value
```

### 兄弟节点动作启发

该算法认为兄弟节点的最佳动作很有可能是当前节点的最佳走法。兄弟节点的深度相同，所以该算法首先在每个深度下，维护一定数目的最佳动作。在后续的遍历中，优先考虑当前深度的最佳动作。

``` python
# 根据历史排序
def compare(action，depth):
    if action in history_table[depth]:
        return 1
    return 0

# 获取state的所有子节点
def sub_nodes(state):
    nodes = []
    action_list = sort(action_list, key=compare, reverse=True) # 对动作进行排序
    for action in action_list: # 获取所有动作
        new_node = do_action(state, action)	  # 计算新节点 
        nodes.append(new_node)
    return nodes 

def negamax_alpha_beta(state, alpha, beta，depth):
    if depth == 0 or state is terminal_node: # 剩余深度为0或叶子节点直接返回静态评估结果
        return estimate(state)  
    value = +∞
    best_action = Null
    for node in sub_nodes(state): #遍历子节点
       	result = -negamax_alpha_beta(node, -beta, -alpha);
        if result > value:
            best_action = node.actoin;  # 记录当前的action
            value = result
            alpha = result 
        if alpha >= beta:
            break;  # 剪枝
    history_table[depth] = append(history_table[depth], best_action) # 更新表
    if len(history_table[depth] > 2):
        del history_table[depth][0] # 如果数目太多，则删除最旧的记录
    return value
```

### 状态动作启发

该算法认为历史上，当前状态状态的最佳动作很可能也是当前的最佳动态。该算法首先在每个状态搜索出最佳动作后，记录状态与动作的映射，在后续的排序中，优先考虑这些动作。

``` python
# 根据历史排序
def compare(action，state):
    if action == history_table[state]:
        return 1
    return 0

# 获取state的所有子节点
def sub_nodes(state):
    nodes = []
    def cmp(action):
        return compare(action,state)
    action_list = sort(action_list, key=cmp, reverse=True) # 对动作进行排序
    for action in action_list: # 获取所有动作
        new_node = do_action(state, action)	  # 计算新节点 
        nodes.append(new_node)
    return nodes 

def negamax_alpha_beta(state, alpha, beta，depth):
    if depth == 0 or state is terminal_node: # 剩余深度为0或叶子节点直接返回静态评估结果
        return estimate(state)  
    value = +∞
    best_action = Null
    for node in sub_nodes(state): #遍历子节点
       	result = -negamax_alpha_beta(node, -beta, -alpha);
        if result > value:
            best_action = node.actoin;  # 记录当前的action
            value = result
            alpha = result 
        if alpha >= beta:
            break;  # 剪枝
    history_table[state] = best_action # 更新表 
    return value
```

## 评估算法

### 静态评估

静态评估不考虑当前节点的子树，只考虑当前状态下，AI与玩家的棋子数量、棋子类型、棋子间的牵制关系、棋子位置。棋子间的牵制关系只考虑棋子可以直接攻击与保护的关系。

``` python

def estimate(state):
    value = 0 
	for chess in get_player_chess(state): # 计算玩家棋子
        value += get_type_value(chess)	# 类型分数
        for target in get_attack_target(chess) # 攻击分数
            value += get_attack_value(target)
        for target in get_defend_target(chess) # 保护分数
            value += get_defend_value(target)
            
	for chess in get_ai_chess(state): # 计算AI棋子
        value -= get_type_value(chess)
        for target in get_attack_target(chess) # 攻击分数
            value -= get_attack_value(target)
        for target in get_defend_target(chess) # 保护分数
            value -= get_defend_value(target)
    return value
```

### 静态搜索算法

当局面出现连续将军、连续交替吃子的情况时，静态评估数值变化大。如果此时贸然进行静态评估，则评估分数非常不准确。所以，为了避免这种情况，在将军或者是吃子时，不能直接评估分数，需要继续搜索子树，但此时只考虑将军与吃子的动作，直至局面不会再将军与吃子。静态搜索算法同样可以限制深度。

``` python
# 当不存在将军与吃子的动作，则局面稳定
def is_stable(state):
    # to do

#只产生产生将军或吃子的子节点
def quiescent_subnodes(states):
    # to do

def quiescent_search(state, alpha, beta):
    if is_stable(state): # 局面稳定则直接返回静态评估结果
        return estimate(state)   
    value = +∞
    for node in quiescent_subnodes(state): #只考虑产生将军或吃子的子节点
        value = max(value, -negamax_alpha_beta(node, -beta, -alpha)) 
        alpha = max(alpha, value)
        if alpha >= beta:
            break;  
    return value     
    
def negamax_alpha_beta(state, alpha, beta):
    if state is terminal_node: # 叶子节点直接返回静态评估结果
        return quiescent_search(state)  # 执行静态搜索
    value = +∞
    for node in sub_nodes(state): #遍历子节点
        value = max(value, -negamax_alpha_beta(node, -beta, -alpha)) # 注意-negamax_alpha_beta(node, -beta, -alpha)的写法
        alpha = max(alpha, value)
        if alpha >= beta:
            break;  # 剪枝
    return value    
```

## 搜索优化算法

### 空窗口探测

在alpha-beta剪枝中，如果alpha，beta窗口十分小，则搜索速度十分高，那么可以通过设置beta=alpha+1，来快速探测节点的估值是>alpha还是<=alpha。如果节点的估值<= alpha，则可以直接进行剪枝。此外，可以据此进行二分搜索，快速找到具体的估值。

``` python
# 快速检测节点的估值范围
def null_window_search(state,alpha):
    return -negamax_alpha_beta(state, alpha,alpha+1);

# 基于空窗探测的二分搜索
def binary_serach(state, alpha, beta):
    # to do 
    
```

### 空着探测

通常情况下，一方的任意行动都会使局面朝着自己的优势发展。那么反过来思考，如果A放弃一次行动，B行动后，A的局势优势任然扩大，那么A的优势非常大，所以不需要深层次的搜索，减少搜索深度。空着探测需要避免连续空着探测的情况。

``` python
def null_move_search(state,alpha,depth):
    if 不会产生连续空着探测 且 浅层次搜索后发现局势优势仍然扩大:
         return negamax_alpha_beta(state, alpha, beta, depth -3) # 减少搜索深度
    else:
        return negamax_alpha_beta(state, alpha, beta, depth) # 常规搜索
```

### 差动作探测

由于子节点的遍历是优化过的，当遍历完一定数量的节点后，后续节点大概率是糟糕的节点，所以可以先进行浅层次地搜索，探测其估值是否在alpha beta窗口内，如果不在窗口内，则直接进行剪枝。

``` python
def negamax_alpha_beta(state, alpha, beta，depth):
    if depth == 0 or state is terminal_node: # 剩余深度为0或叶子节点直接返回静态评估结果
        return estimate(state)  
    value = +∞
    best_action = Null
    action_count = 0
    for node in sub_nodes(state): #遍历子节点
        actoun_count += 1
        if action_count > 5 and depth > 3: # 如果已经搜索了一定数量的节点且剩余的搜索高度大于一定值，则进行探测
            if(null_window_search(state,alpha,depht-3)<=alpha): 
                continue; # 直接剪枝
       	result = -negamax_alpha_beta(node, -beta, -alpha);
        if result > value:
            best_action = node.actoin;  # 记录当前的action
            value = result
            alpha = result 
        if alpha >= beta:
            break;  # 剪枝
    history_table[best_action] += depth * depth # 更新权值, 增加的权值与depth有关，depth越大，权值越大
    return value
```

### 历史表缓存

对于每一次alpha-beta搜索，我们可以得到当前state的估值范围，其要么是小与等于alpha，要么是大于等于beta，要么是alpha-beta中具体的某个值。所以得到搜索结果后，可以将其与搜索深度进行缓存。当遇到重复的state后，查询缓存，可以减少alpha beta窗口或者直接剪枝。

## 哈希算法

### 棋盘局面哈希 -- Zobrist

由于棋盘局面的哈希使用得非常频繁，所以需要一个快速的哈希算法，其能够快速的通过动作计算出父节点或子节点的哈希值。 

``` python
# 根据棋子颜色、类型、位置产生一个恒定的唯一的hash值
  
def hash(color,type,position):
    # to do

def update_hash(old_hash, chess):
    return old_hash ^ hash(chess.color, chess.type, chess.position) 

def get_next_hash(old_hash, action):
    old_hash = update_hash(old_hash, action.from_chess)
    old_hash = update_hash(old_hash, action.to_chess)
    old_hash = update_hash(old_hash, action.new_from_chess)
    old_hash = update_hash(old_hash, action.new_to_chess)
    return old_hash
```

