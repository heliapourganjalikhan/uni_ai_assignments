
% Example usage
weights = [2, 3, 4, 5];       % Weights of the items
values = [3, 4, 5, 6];        % Values of the items
capacity = 5;                 % Maximum weight capacity of the knapsack
numSailfish = 30;             % Number of sailfish
maxIter = 100;                % Maximum number of iterations

sfo_knapsack = SailfishOptimizerKnapsack(weights, values, capacity, numSailfish, maxIter);
[solution, fitness] = sfo_knapsack.optimize();

disp('Best Solution (Item Inclusion):');
disp(solution);
disp('Total Value:');
disp(fitness);
