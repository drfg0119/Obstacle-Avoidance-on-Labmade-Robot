% Create DDPG agent and training options for walking robot example
%
% Copyright 2019 The MathWorks, Inc.

%% DDPG Agent Options
agentOptions = rlDDPGAgentOptions;
Ts=0.1; Tf=30; %added by renyo
agentOptions.SampleTime = Ts;
agentOptions.DiscountFactor = 0.99;
%agentOptions.MiniBatchSize = 128;
agentOptions.MiniBatchSize = 64;
%agentOptions.ExperienceBufferLength = 5e5;
agentOptions.ExperienceBufferLength = 1e5;

agentOptions.TargetSmoothFactor = 1e-3;
agentOptions.NoiseOptions.MeanAttractionConstant = 5;
agentOptions.NoiseOptions.Variance = 0.5;
agentOptions.NoiseOptions.VarianceDecayRate = 1e-5;

%% Training Options
trainingOptions = rlTrainingOptions;
trainingOptions.MaxEpisodes = 500000;
trainingOptions.MaxStepsPerEpisode = Tf/Ts;
trainingOptions.ScoreAveragingWindowLength = 100;
trainingOptions.StopTrainingCriteria = 'AverageReward';
trainingOptions.StopTrainingValue = 5000000;
trainingOptions.SaveAgentCriteria = 'EpisodeReward';
trainingOptions.SaveAgentValue = 600000;
trainingOptions.Plots = 'training-progress';
trainingOptions.Verbose = true;
% if useParallel
%     trainingOptions.Parallelization = 'async';
%     trainingOptions.ParallelizationOptions.StepsUntilDataIsSent = 32;
% end