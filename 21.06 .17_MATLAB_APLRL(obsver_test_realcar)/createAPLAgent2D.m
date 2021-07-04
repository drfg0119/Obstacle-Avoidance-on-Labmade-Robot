% APL -- DDPG Agent Training Script (2D)
% Copyright 2019 The MathWorks, Inc.

%% SET UP ENVIRONMENT
% Speedup options
%useFastRestart = true;
useGPU = true;
useParallel = true;

% Create the observation info
numObs = 7;
observationInfo = rlNumericSpec([numObs 1]);
observationInfo.Name = 'observations';

% create the action info
numAct = 2;
actionInfo = rlNumericSpec([numAct 1],'LowerLimit',[0.1;-0.2],'UpperLimit', [0.3;0.2])
actionInfo.Name = 'linear_vel;angular_vel';

% Environment
mdl = 'APLRL2D_avoidance_version';
%mdl = 'APLRL_test_ObserveNum';
load_system(mdl);
blk = [mdl,'/RL Agent'];
env = rlSimulinkEnv(mdl,blk,observationInfo,actionInfo)
%env.ResetFcn = @(in)APLResetFcn();
%env.ResetFcn = APLResetFcn();
% if ~useFastRestart
   env.UseFastRestart = 'off';
% end

%% CREATE NEURAL NETWORKS
createDDPGNetworks;

%% CREATE AND TRAIN AGENT
createDDPGOptions;
%agent = rlDDPGAgent(actor,critic,agentOptions);
%agent = saved_agent;
trainingResults = train(agent,env,trainingOptions)

%% Clears the experience buffer
reset(agent);
%% SAVE AGENT
%reset(agent); % Clears the experience buffer
curDir = pwd;
saveDir = 'savedAgents';
cd(saveDir)
save(['trainedAgent_APLRL_' datestr(now,'mm_DD_YYYY_HHMM')],'agent');
save(['trainingResults_APLRL_' datestr(now,'mm_DD_YYYY_HHMM')],'trainingResults');
cd(curDir)
%%
plotTrainingResults