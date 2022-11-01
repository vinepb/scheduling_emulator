function [best amount] = knapsack(weights, values, W)

%tamanho da matriz
weights = weights(:);
values = values(:);
lengths = length(weights);
   
%Matriz com zeros
  V = zeros(lengths+1,W+1);
  
%preenchendo a matriz
  for j=2:lengths+1;
    for Y=2:W+1;
      if(weights(j-1)>=Y)
        V(j,Y)=V(j-1,Y);
      else
        a=[V(j-1,Y) values(j-1)+V(j-1,Y-weights(j-1))];
        V(j,Y)=max(a);
      end
    end
  end
best = V(end,end);
   

%Colocando itens colocados na mochila
K= W+1;
for j=lengths:-1:1
  if V(j,K) == best
       amount(j)=0;
  else
       amount(j) = 1;
       K = K - weights(j);
       best = V(j,K);
 
    end
end
end