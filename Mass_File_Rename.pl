#!usr/local/bin/perl

#=====================================================================================================#

###		Mass File Renaming Tool

###		USE IT WITH EXTREME CARE

###		ONCE RENAMED.NO FILE NAME CAN BE REVERTED

###		Script to Modify Only the File Names in a Directory from Abc#DEf-gHi.TxT to Abc_Def_Ghi.txt

###		Traverses to All the Sub Directory Files also and Will Rename

###		Will Not Rename Any Directories

###		POSIX Compatible.Works with Linux,Windows and Mac File System.But Tested Only in Linux and Windows

###		Author: Vishnu Vasan Nehru

###		24.12.2012

#=====================================================================================================#
use Cwd;

my ($old,$ren,$new1,$directory,$dir_exist,@dirs,@split,@word_split,@new,@files,$tmp,$tmp1);$dir_exist=0;

@files=<*>;

map 
{
	if(-d $_)
	{
		$cwd=cwd;
		if($^O=~/(.*)win(.*)/i){$cwd=~s/\//\\/g;$tmp="$cmd\\$_";}
		elsif($^O=~/(.*)linux(.*)|(.*)mac(.*)/i){$cwd=~s/\\/\//g;$tmp="$cmd\/$_";}
		push(@dirs,$tmp);
	}
} @files;
_:
map
{
	if (!(-d $_))
	{$old=$_;			
		if(($_!~/$0/i)&&($_!~/^\~/))
		{
			# Pattern Responsible for the Name Split.Modify it according to your need.If it finds any of the below mentioned match it will replace with the mentioned character
			@split=split(/\!|\#|\@|\$|\%|\^|\&|\*|\(|\)|\-|\_|\+|\{|\}|\||\[|\]|\"|\'|\;|\:|\?|\<|\>|\\|\/|\,/,$_);				
			map{
					@word_split=split(//,$_);
					@word_split=map{lc($_)} @word_split;
					$word_split[0]=uc($word_split[0]);
					push(@new,join('',@word_split));
				} @split;
			#Underscore character in Join Statement is the Replacement Character.
			#Modify the Underscore '_' Character in Join Statement to Change the New Modifier you want to replace with.
			$new1=join('_',@new);@new=();
		}	
		if(($old ne $new1)&&($old ne $0))
		{
			if($^O=~/(.*)win(.*)/i){$ren="rename \'$old\' $new1";}
			elsif($^O=~/(.*)linux(.*)|(.*)mac(.*)/i){$ren="mv \'$old\' $new1";}
			system($ren);
		}	
	}
} 
@files;
map
{
	$directory=$_;
	shift(@dirs);
	if ($directory!~/""/){$dir_exist=1;}
	goto __;
} @dirs;
__:
if($dir_exist==1)
{
	chdir($directory);
	@files=<*>;
	map
	{
		$cwd=cwd;
		if(-d $_)
		{
			if($^O=~/(.*)win(.*)/i){$cwd=~s/\//\\/g;$tmp1="$cwd\\$_";}
			elsif($^O!~/(.*)linux(.*)|(.*)mac(.*)/i){$cwd=~s/\\/\//g;$tmp1="$cwd\/$_";}
			unshift(@dirs,$tmp1);
		}
	}@files;
	$dir_exist=0;
	goto _;
}
