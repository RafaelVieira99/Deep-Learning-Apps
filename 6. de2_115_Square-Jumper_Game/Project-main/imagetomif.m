image_name='mainmenu.png';
text_name='mainmenumif.mif';


im = imread(image_name);
im = imresize(im,[32 32]);

fid = fopen(text_name,'W'); 

imsize = size(im);

fprintf(fid,'WIDTH = 12;\n');
fprintf(fid,'DEPTH = %d;\n',imsize(1)*imsize(2));
fprintf(fid,'ADDRESS_RADIX = BIN;\n');
fprintf(fid,'DATA_RADIX = BIN;\n');
fprintf(fid,'CONTENT BEGIN\n\n');




count = 0;
if(fid)
    for i=1:imsize(1)
        for j=1:imsize(2)

            
            str=num2str(de2bi(count,10,'left-msb'));
            str=str(find(~isspace(str)));
            fprintf(fid,'%s:',str);
            count = count + 1;

            for k = 1:3
                numero = double(im(i,j,k))/255;
                numero = numero*15;
                numero = round(numero);
                numero = num2str(de2bi(numero,4,'left-msb'));
                numero=num2str(numero);
                numero=numero(find(~isspace(numero)));
                fprintf(fid,'%s',numero);
            end
            fprintf(fid,';\n');
        end 

    end
    fprintf(fid,'END;\n');
    fclose(fid);
end