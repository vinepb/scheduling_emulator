function [best amount] = knapsack(weights, values, max_weight)
    % Matrix size.
    weights = weights(:);
    values = values(:);
    lengths = length(weights);
    % Initialize all values of the matrix to 0.
    aux_matrix = zeros(lengths + 1, max_weight + 1);
    % Fill the matrix with the values of the knapsack problem.
    for j = 2:lengths + 1;
        for i = 2:max_weight + 1;
            if (weights(j - 1) >= i)
                aux_matrix(j, i) = aux_matrix(j - 1, i);
            else
                aux_matrix(j, i) = max([aux_matrix(j - 1, i) values(j - 1) + aux_matrix(j - 1, i - weights(j - 1))]);
            end
        end
    end
    best = aux_matrix(end, end);
    % Find the items that will be in the knapsack.
    k = max_weight + 1;
    for j = lengths:-1:1
        if aux_matrix(j, k) == best
            amount(j) = 0;
        else
            amount(j) = 1;
            k = k - weights(j);
            best = aux_matrix(j, k);
        end
    end
end