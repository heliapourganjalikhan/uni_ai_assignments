classdef SailfishOptimizerKnapsack
    properties
        weights;          % Weights of the items (array)
        values;           % Values of the items  (array )
        capacity;         % Maximum capacity of the knapsack
        numSailfish;      % Number of sailfish
        maxIter;          % Maximum number of iterations
        sailfishPositions; % Positions of sailfish (solutions)
        % A matrix representing the current population of solutions.
        % Each item row corresponds to a sailfish, and each column
        % corresponds to an item (0 or 1 indicating wether the item is excluded or included in the knapsack)
    end
    
    methods
        % The constructor
        function obj = SailfishOptimizerKnapsack(weights, values, capacity, numSailfish, maxIter)
            % Assigns the provided values
            obj.weights = weights;
            obj.values = values;
            obj.capacity = capacity;
            obj.numSailfish = numSailfish;
            obj.maxIter = maxIter;
            % Initialize the population
            obj.sailfishPositions = obj.initializePopulation();
        end
        
        % This method generates a random initial population of sailfish
        % solutions
        function positions = initializePopulation(obj)
            % Generate random binary solutions (0 or 1 for each item)
            positions = randi([0, 1], obj.numSailfish, length(obj.weights));
        end
        
        % This method calvulates the "fotness" of a given solution
        function fitnessValue = fitness(obj, solution)
            % calculations:
            totalWeight = sum(solution .* obj.weights);
            totalValue = sum(solution .* obj.values);
            % logic:
            if totalWeight > obj.capacity
                fitnessValue = 0; % Invalid solution exceeds capacity
            else
                fitnessValue = totalValue; % Valid solution value
            end
        end
        
        % The main algorithm for optimizing the solutions
        function [bestSolution, bestFitness] = optimize(obj)
            % Initialize the best solution:
            bestSolution = []; % No solution selected initially
            bestFitness = 0; % Initialize with zero
            
            % This loop improves the solutions over iterations:
            for iter = 1:obj.maxIter
                for i = 1:obj.numSailfish
                    % For each sailfish, calculate its fitness
                    currentFitness = obj.fitness(obj.sailfishPositions(i, :));

                    % If a sailfish has a better fitness than the current bestFitness, update bestSolution and bestFitness
                    if currentFitness > bestFitness
                        bestFitness = currentFitness;
                        bestSolution = obj.sailfishPositions(i, :);
                    end
                end
                
                % Update sailfish positions based on the best solution
                for i = 1:obj.numSailfish
                    % With 50% probability, copy the bestSolution
                    if rand < 0.5
                        obj.sailfishPositions(i, :) = bestSolution;
                    % Otherwise, apply a mutation using the mutate method
                    else
                        obj.sailfishPositions(i, :) = obj.mutate(obj.sailfishPositions(i, :));
                    end
                end
            end
        end
        
        % This method introduces randomness by flipping a single bit in the solution:
        function newSolution = mutate(obj, solution)
            % Randomly flip a bit in the solution
            index = randi(length(solution));
            newSolution = solution;
            newSolution(index) = 1 - newSolution(index); % Flip 0 to 1 or 1 to 0
        end
    end
end
