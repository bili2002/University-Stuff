let g:keystroke_count = -1
let g:temp_keystroke_count = 0
let g:excluded_commands = ['GetCurrent', "SaveCheckpoint", "LoadCheckpoint"] 

function! KeystrokeCounterCommand()
    let g:temp_keystroke_count += 1
endfunction


function! KeystrokeCounter()
  let g:keystroke_count += 1
endfunction


function! LeavingCmd()
  let excluded = index(g:excluded_commands, split(histget(':', -1))[0]) != -1
  if !excluded
    let g:keystroke_count += g:temp_keystroke_count + 2
  endif
  
  let g:temp_keystroke_count = 0
endfunction


augroup KeystrokeCounter
  autocmd!
  autocmd CmdlineChanged  * call KeystrokeCounterCommand()
  autocmd CursorMoved,CursorMovedI,InsertEnter * call KeystrokeCounter()
  autocmd CmdlineLeave * call LeavingCmd()
  autocmd ModeChanged *:[vV\x16]* call KeystrokeCounter()
  autocmd Modechanged [vV\x16]*:* call KeystrokeCounter()
  
augroup END

command! GetCurrent echo g:keystroke_count

set laststatus=2
set statusline+=\%{g:keystroke_count}\ keystrokes





command! -nargs=1 SaveCheckpoint :call SaveCopyToFile(<f-args>)
function! SaveCopyToFile(filename)
  let target_directory = $HOME . '/.hidden_directory'
  let target_path = target_directory . '/' . a:filename
  let target_path_ks = target_directory . '/' . a:filename . "_ks"

  if !isdirectory(target_directory)
    call mkdir(target_directory, 'p')
  endif

  silent execute 'write! ' . target_path
  call writefile([g:keystroke_count], target_path_ks)
  
  echohl MoreMsg
  echo 'Saved checkpoint: ' . a:filename
  echohl None
  
  silent execute 'write! ' . target_path
endfunction


command! -nargs=1 LoadCheckpoint :call LoadCopyFile(<f-args>)
function! LoadCopyFile(filename)
  let target_directory = $HOME . '/.hidden_directory'
  let target_path = target_directory . '/' . a:filename
  let target_path_ks = target_directory . '/' . a:filename . "_ks"

  if filereadable(target_path)
	let g:keystroke_count = str2nr(readfile(target_path_ks)[0])
	
    silent execute "normal! ggVGd"
    call setline(1, readfile(target_path))

	echohl MoreMsg
    echo 'Loaded checkpoint: ' . a:filename
    echohl None
	
  else
    echohl ErrorMsg
    echo 'Checkpoint not found: ' . a:filename
    echohl None
	
  endif
endfunction