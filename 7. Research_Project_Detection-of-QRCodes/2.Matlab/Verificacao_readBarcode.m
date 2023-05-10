clear;clc;

myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Darker-Light-Condition\Draft-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Darker Light','Draft Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = verifica(:,:);
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Darker-Light-Condition\High-Quality-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Darker Light','High Quality Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Darker-Light-Condition\Standard-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Darker Light','Standard Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Low-Light\Draft-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Low Light','Draft Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Low-Light\High-Quality-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Low Light','High Quality Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Low-Light\Standard-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Low Light','Standard Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Not-so-Low-Light\Draft-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Not so Low Light','Draft Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Not-so-Low-Light\High-Quality-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Not so Low Light','High Quality Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Papel-Normal-80g\Not-so-Low-Light\Standard-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Papel Normal 80g','Not so Low Light','Standard Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Photographic-Glossy-Paper\Darker-Light-Condition'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Photographic Glossy Paper','Darker Light','-', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Photographic-Glossy-Paper\Low-Light'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Photographic Glossy Paper','Low Light','-', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Photographic-Glossy-Paper\Normal-Room-Light'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Photographic Glossy Paper','Normal Room Light','-', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Rough-Paper\Darker-Light-Condition'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Rough Paper','Darker Light','-', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Rough-Paper\Low-Light'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Rough Paper','Low Light','-', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Rough-Paper\Normal-Room-Light'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Rough Paper','Normal Room Light','-', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Selos-Descaracterizados- Digital'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Digital','-','-', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
  
end

verificanew = vertcat(verificanew(:,:), verifica(:,:));
%%
myFolder = 'QR-Codes-Dataset\Special-Case-2lines-removed\High-Quality-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Special Case 2lines removed','-','High Quality Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
  
end

verificanew = vertcat(verificanew(:,:), verifica([1,2],:));
%%
myFolder = 'QR-Codes-Dataset\Special-Case-2lines-removed\Low-Quality-Printing'; 
 
 if ~isdir(myFolder)
     errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
     uiwait(warndlg(errorMessage));
     return
 end
filePattern = fullfile(myFolder, '*.jpg');
jpegFiles = dir(filePattern);

for k = 1:length(jpegFiles)
  baseFileName = jpegFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  imageArray = imread(fullFileName);
  %figure;
  %imshow(imageArray);  % Display image.
  msg = readBarcode(imageArray);
  
  if msg == "http://www.uc.pt"
     check = 1;
  else
     check = 0;
  end
  C = {'Special Case 2lines removed','-','Low Quality Printing', baseFileName, check};
  verifica(k,:) = cell2table(C,'VariableNames',{'Tipo','Condição','Qualidade','Nome','Leu o QR'});
  
end

verificanew = vertcat(verificanew(:,:), verifica([1,2],:));