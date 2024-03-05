# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/04 15:40:17 by okraus            #+#    #+#              #
#    Updated: 2024/03/05 12:40:58 by plouda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ifndef ECHO
# HIT_TOTAL	!=	${MAKE} ${MAKECMDGOALS} --dry-run ECHO="HIT_MARK" | grep -c "HIT_MARK"
# HIT_COUNT1	=	$(eval HIT_N != expr ${HIT_N} + 1)${HIT_N}
# HIT_COUNT2	=	$(eval HIT_M != expr ${HIT_M} + 1)${HIT_M}
# HIT_COUNT3	=	$(eval HIT_O != expr ${HIT_O} + $(BAR_LENGTH))${HIT_O}
# PERC		=	$(shell expr $(HIT_COUNT3) / $(HIT_TOTAL) | bc)
# ECHO		=	echo "[`expr ${HIT_COUNT1} '*' 100 / ${HIT_TOTAL}`%]"
# endif

NAME		=	miniRT
LIBFT_F		=	libft/
LIBFT		=	libft.a
HEADER		=	header/minirt.h
LIBMLX		=	./.MLX42
LIBS		=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
#LIBS		=	$(LIBMLX)/build/libmlx42.a -Iinclude -lglfw
LHEADERS	=	-I ./include -I $(LIBMLX)/include
CC 			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g -O2
CMFLAGS		=	-DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -B build
OBJ_DIR		=	objs/
SLEEP		=	#sleep 0.5

#CCC		:= cc
#CFLAGS	:= -Wextra -Wall -Werror -Wunreachable-code -Ofast
#


#all: libmlx $(NAME)

#libmlx:
#	@cmake $(LIBMLX) $(CMFLAGS) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4
#%.o: %.c
#	@$(CCC) $(CFLAGS) -o $@ -c $< $(LHEADERS) && printf "Compiling: $(notdir $<)"
#$(NAME): $(OBJS)
#	@$(CCC) $(OBJS) $(LIBS) $(LHEADERS) -o $(NAME)
# SOURCES

SRC			=	$(addprefix $(SRC_DIR), $(SRC_S)) \
				$(addprefix $(SRC_MDIR), $(SRC_M)) \
				$(addprefix $(SRC_ODIR), $(SRC_O)) \
				$(addprefix $(SRC_RDIR), $(SRC_R)) \
				$(addprefix $(SRC_UDIR), $(SRC_U)) \
				$(addprefix $(SRC_CDIR), $(SRC_C)) \
				$(addprefix $(SRC_SHDIR), $(SRC_SH)) \
				$(addprefix $(SRC_HOOKDIR), $(SRC_HOOKS)) \
				$(addprefix $(SRC_FILEDIR), $(SRC_FILE)) \
				$(addprefix $(SRC_OBJCHDIR), $(SRC_OBJCH)) \
				$(addprefix $(SRC_OBJMANDIR), $(SRC_OBJMAN))


# Source directories

SRC_DIR			=	src/
SRC_MDIR		=	src/maths/
SRC_ODIR		=	src/objects/
SRC_RDIR		=	src/rays/
SRC_UDIR		=	src/utils/
SRC_CDIR		=	src/camera/
SRC_SHDIR		=	src/shader/
SRC_HOOKDIR		=	src/hooks/
SRC_FILEDIR		=	src/file/
SRC_OBJCHDIR	=	src/object_choice/
SRC_OBJMANDIR	=	src/object_manipulation/

# miniRT functions

SRC_S		=	minirt.c \
				objlist.c

SRC_M		=	quaternions.c \
				quaternions2.c \
				quaternions3.c \
				vector_ops.c \
				vector_ops2.c

SRC_O		=	fill_object_getters.c \
				o_ambient.c \
				o_camera.c \
				o_cylinder.c \
				o_cylinder_disc.c \
				o_plane.c \
				o_sphere.c \
				o_cone.c \
				o_cone_disc.c \
				o_light_sphere.c \
				o_checkerboard.c \
				object_handler.c \
				set_texture_data_pointers.c \
				o_texture.c \
				o_vector_map.c \
				set_object_vectors.c
				
SRC_R		=	find_rays.c \
				intersections.c

SRC_U		=	check_identifiers_helpers.c \
				error.c \
				format_check_utils.c \
				format_check_utils2.c \
				math_utils.c \
				print_contents.c \
				utils1.c \
				ft_atof.c

SRC_C		= 	camera.c \
				movements.c \
				movements2.c \
				rotations.c \
				quaternion_rotations.c \
				camera_in_objects.c \
				camera_in_objects_functions.c

SRC_SH		=	object_shaders.c \
				shader_utils.c \
				shader_utils2.c \
				phong_illumination.c

SRC_HOOKS	=	draw_string_in_window.c \
				hooks.c \
				hooks_helpers.c

SRC_FILE	=	load_file.c \
				get_texture_data.c \
				get_object_data.c

SRC_OBJCH	=	change_modes_cylinder_cone.c \
				choose_object_by_keys.c \
				choose_object_by_mouse_reference.c \
				object_iterators.c \
				reset_to_default.c

SRC_OBJMAN	=	change_properties.c \
				manipulate_light.c \
				manipulate_objects.c \
				manipulated_objects.c


# Formating

NRM_FORMAT	=	\033[0m

GREEN		=	\033[1;32m
YELLOW		=	\033[1;93m
RED			=	\033[1;31m
REVERSE		=	\033[7m
RETURN		=	#\033[1F\r\033[2K

# Printing

# BAR_LENGTH	:=	50
# PRINT1		=	printf "[$(HIT_COUNT2)/$(HIT_TOTAL)]" ;
# PRINT2		=	for num in `seq 1 $(BAR_LENGTH)` ; do \
# 					if [ $$num -le $(PERC) ] ; then \
# 						printf "$(REVERSE) $(NRM_FORMAT)" ; \
# 					else \
# 						printf " " ; \
#         			fi \
#     			done

# Objects

OBJ			=	$(addprefix $(OBJ_DIR), $(SRC_S:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_M:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_O:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_R:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_U:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_C:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_SH:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_HOOKS:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_FILE:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_OBJCH:.c=.o)) \
				$(addprefix $(OBJ_DIR), $(SRC_OBJMAN:.c=.o))

# RULES

all:			announce libmlx $(LIBFT) $(NAME)
				@echo "$(RETURN)$(RETURN)$(GREEN)miniRT compiled! $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)
				@echo "$(GREEN)Amazing <<$(REVERSE)miniRT$(NRM_FORMAT)$(GREEN)>> compiled!$(NRM_FORMAT)"
				

bonus:			all

# MESSAGES 

announce: 
				@echo "$(RED)Magic in progress...\n$(NRM_FORMAT)"

#LIBMLX

libmlx:
				@cmake $(LIBMLX) $(CMFLAGS) -B $(LIBMLX)/build && make -s -C $(LIBMLX)/build -j4
				@echo "$(GREEN)The <<$(REVERSE)MLX42$(NRM_FORMAT)$(GREEN)>> compiled!$(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# COMPILATION

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_MDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_ODIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_RDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_UDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_CDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_SHDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_HOOKDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_FILEDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_OBJCHDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(OBJ_DIR)%.o:	$(SRC_OBJMANDIR)%.c $(HEADER)
				@mkdir -p $(OBJ_DIR)
				@$(SLEEP)
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(CC) $(CFLAGS) -c $< -o $@
				@$(ECHO)

$(NAME): 		$(OBJ)
				@echo "$(RETURN)$(RETURN)$(GREEN)Libft compilation complete!$(NRM_FORMAT)"
				@$(CC) $(CFLAGS) $(SRC) libft.a $(LIBS) $(LHEADERS) -o $(NAME)
				@echo "$(RETURN)$(RETURN)$(GREEN)miniRT compiled!$(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

$(LIBFT):
				@echo "$(RETURN)$(RETURN)$(YELLOW)Compiling LIBFT: $< $(NRM_FORMAT)"
				@make -s -C $(LIBFT_F)
				@cp libft/libft.a .
				@echo "$(YELLOW)Compiling miniRT: $< $(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# clean library

clean:
				@rm -rf $(OBJ_DIR)
				@rm $(LIBFT)
				@make fclean -s -C $(LIBFT_F)
				@rm -rf $(LIBMLX)/build
				@echo "$(RED)Objects removed!$(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# Remove the program after running clean

fclean:			clean
				@rm -f $(NAME)
				@echo "$(RETURN)$(RED)Library and program deleted!$(NRM_FORMAT)"
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# Remove stuff and make it all again

re: 			fclean all
				@$(PRINT2)
				@$(PRINT1)
				@$(ECHO)

# Phony stuff for rules

.PHONY: all announce clean fclean re bonus
